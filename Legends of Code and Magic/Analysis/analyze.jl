
 struct Card
      id::Int
      name::String
      type::Int
      cost::Int
      dmg::Int
      hp::Int
      breakthrough::Bool
      charge::Bool
      drain::Bool
      guard::Bool
      Lethal::Bool
      ward::Bool
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
      type = c[3] == "creature" ? 0 : c[3] == "itemGreen" ? 1 : c[3] == "itemRed" ? 2 : 3
      cost = parse(Int, c[4])
      dmg = parse(Int, c[5])
      hp = parse(Int, c[6])
      breakthrough = any(x->x=='B', c[7])
      charge = any(x->x=='C', c[7])
      drain = any(x->x=='D', c[7])
      guard = any(x->x=='G', c[7])
      lethal = any(x->x=='L', c[7])
      ward = any(x->x=='W', c[7])
      myhc = parse(Int, c[8])
      ophc = parse(Int, c[9])
      draw = parse(Int, c[10])
      return Card(id,name,type,cost,dmg,hp,breakthrough,charge,drain,guard,lethal,ward,myhc,ophc,draw)
end

toInts(ca)=[ca.cost, ca.type == 0, ca.type == 1, ca.type == 2, ca.type == 3, ca.dmg, ca.hp, ca.breakthrough, ca.charge, ca.drain, ca.guard, ca.Lethal, ca.ward, ca.myhc, ca.ophc, ca.draw]

function getCards()
      return map(makeCard, [split(e, " ; ") for e in split(readCardList(), '\n')][1:end-1])
end
