object T2Viterbi {
  private case class HState(
    name: Char,
    transToOpen: Map[Char, Double],
    transToHidden: Map[Char, Double],
    sigma: Double,
    phi: Char
  )

  private def processChar(curList: List[HState], ch: Char): List[HState] = {
    val newList = curList.map(state => {
      val nSigmaPhi = curList
        .map(st => (st.sigma * st.transToHidden.getOrElse(state.name, Double.NaN) * state.transToOpen(ch), st.name))
        .maxBy(_._1)
      HState(state.name, state.transToOpen, state.transToHidden, nSigmaPhi._1, nSigmaPhi._2)
    })
    newList
  }

  def main(args: Array[String]): Unit = {
    val s = scala.io.StdIn.readLine()
    val prompterMap = Map('A' -> 0.1, 'T' -> 0.1, 'C' -> 0.4, 'G' -> 0.4)
    val nonPrompterMap = Map('A' -> 0.3, 'T' -> 0.3, 'C' -> 0.2, 'G' -> 0.2)
    val initPrompt =
      HState('P', prompterMap, Map('P' -> 0.9, 'N' -> 0.1), prompterMap.getOrElse(s.charAt(0), 0.0) * 0.5, '-')
    val initNonPrompt =
      HState('N', nonPrompterMap, Map('P' -> 0.2, 'N' -> 0.8), nonPrompterMap.getOrElse(s.charAt(0), 0.0) * 0.5, '-')
    val states = List(List(initPrompt, initNonPrompt))
    val revTable = s.tail.foldLeft(states)((l: List[List[HState]], ch: Char) => processChar(l.head, ch) :: l)
    val seq = revTable.tail.foldLeft(List(revTable.head.maxBy(_.sigma)))((accum: List[HState], cur: List[HState]) => {
      cur.filter(state => state.name == accum.head.phi).head :: accum
    })
    println(seq.map(_.name).mkString)
  }
}
