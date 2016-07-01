<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Informações sobre os perfis atuantes na formação: Idade</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

 


echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "select 
			uniabrev||' - '||unidsc as universidade, 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curso,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end as secretaria,
			'F'||s.siftipoplanejamento as fase,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and extract(year from age(i.iusdatanascimento)) >= 60) as id1,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and (extract(year from age(iusdatanascimento)) >= 50 AND extract(year from age(iusdatanascimento)) < 60)) as id2,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and (extract(year from age(iusdatanascimento)) >= 40 AND extract(year from age(iusdatanascimento)) < 50)) as id3,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and (extract(year from age(iusdatanascimento)) >= 30 AND extract(year from age(iusdatanascimento)) < 40)) as id4,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and (extract(year from age(iusdatanascimento)) >= 20 AND extract(year from age(iusdatanascimento)) < 30)) as id5,
			(select count(*) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and (extract(year from age(iusdatanascimento)) <= 19)) as id6

		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		where sifstatus='A'  AND d.esdid='1187' ORDER BY 2,1";

$cabecalho = array("IES","Curso","Secretaria","Fase","60+","50-59","40-49","30-39","20-29","19-");

$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');


echo '</td>';
echo '</tr>';
echo '</table>';

 
?>