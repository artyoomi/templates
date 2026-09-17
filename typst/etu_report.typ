/*
Template according to: https://se.moevm.info/doku.php/courses:reportrules

Latex reference from one cool guy:
https://github.com/JAkutenshi/eltechLaTeXTemplates/blob/master/LabReports/tex/title.tex
*/


// Page setup
#set page(
  width: 210mm,
  height: 297mm,
  margin: (top: 20mm, bottom: 20mm, left: 30mm, right: 15mm)
)

// General text setup
#set text(
  size: 14pt,
  lang: "ru"
)

// Paragraph setup
#set par(
  leading: 1.5em,
  first-line-indent: 1.25cm,
  justify: true
)

// To provide numeration like 1, 1.1, 1.1.1 and so on
#set enum(full: true)

// Setup level 1 header
#show heading.where(level: 1): it => [
  #set text(size: 14pt, weight: "bold")
  #set par(first-line-indent: 0pt, leading: 1.5em)
  #set align(center)
  #upper(it.body)
]

// Setup level 2 header
#show heading.where(level: 2): it => [
  #set text(size: 14pt, weight: "bold")
  #set par(first-line-indent: 1.25cm, leading: 1.5em, justify: true)
  #it.body
]

// Setup table captions
#show figure.where(kind: table): fig => {
  align(left)[
    #fig.caption
    #fig.body
  ]
}

// Long "-" between numering and caption in all figures
#show figure: set figure.caption(separator: [ --- ])

// Force all raw blocks to have 1em indent between lines
#show raw.where(block: true): set par(leading: 1em)
// Force all raw blocks to have left alignment
#show raw.where(block: true): set align(left)

// Enable formula numbering
#set math.equation(numbering: "(1)")

// First page setup
#align(center)[
  #set text(weight: "semibold")

  #set par(leading: 1em)

  МИНОБРНАУКИ РОССИИ \
  САНКТ-ПЕТЕРБУРГСКИЙ ГОСУДАРСТВЕННЫЙ \
  ЭЛЕКТРОТЕХНИЧЕСКИЙ УНИВЕРСИТЕТ \
  «ЛЭТИ» ИМ. В.И. УЛЬЯНОВА (ЛЕНИНА) \
  Кафедра БЖД

  #v(54mm)

  ОТЧЕТ \
  по лабораторной работе №1 \
  по дисциплине «Дисциплина» \
  Тема:  Тема

  #v(54mm)

  #table(
    columns: (33%, 33%, 33%),
    inset: 10pt,
    align: horizon,
    stroke: none,
    "Студенты гр. 3381",
    "",
    table.hline(start: 1 , end: 2),
    "Иванов И.И.",
    "Преподаватель",
    "",
    table.hline(start: 1 , end: 2),
    "Иванов И.И."
  )

  #set align(bottom)
  Санкт-Петербург \
  #datetime.today().year()
]
#pagebreak()

// Start numbering here to skip first page numering
#set page(
  numbering: "1"
)


// To make indent before first header
\
== Задание


\
== Выполнение работы


\
== Вывод

