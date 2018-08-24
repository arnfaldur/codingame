
 struct Card
      id::Int
      name::String
      type::Int
      cost::Int
      dmg::Int
      hp::Int
      breakthrough::Bool
      Charge::Bool
      Drain::Bool
      Guard::Bool
      Lethal::Bool
      Ward::Bool
      myhc::Int
      ophc::Int
      draw::Int
end

function readCardList(file="cardlist.txt")
      f = open(file)
      a = read(f, String)
      close(f)
      return a
end

function makeCard(c)
      id = parse(Int,c[1])
      name = c[2]
      type = c[3] == "Creature" ? 0 : c[3] == "itemGreen" ? 1 : c[3] == "itemRed" ? 2 : 3
      cost = parse(Int, c[4])
      dmg = parse(Int, c[5])
      hp = parse(Int, c[6])
      breakthrough = any(x->x=='B', c[7])
      Charge = any(x->x=='C', c[7])
      Drain = any(x->x=='D', c[7])
      Guard = any(x->x=='G', c[7])
      Lethal = any(x->x=='L', c[7])
      Ward = any(x->x=='W', c[7])
      myhc = parse(Int, c[8])
      ophc = parse(Int, c[9])
      draw = parse(Int, c[10])
      return Card(id,name,type,cost,dmg,hp,breakthrough,Charge,Drain,Guard,Lethal,Ward,myhc,ophc,draw)
end

toInts(ca)=[ca.cost, ca.type == 0, ca.type == 1, ca.type == 2, ca.type == 3, ca.dmg, ca.hp, ca.breakthrough, ca.Charge, ca.Drain, ca.Guard, ca.Lethal, ca.Ward, ca.myhc, ca.ophc, ca.draw]

function getCards()
      return map(makeCard, [split(e, " ; ") for e in split(readCardList(), '\n')][1:end-1])
end
