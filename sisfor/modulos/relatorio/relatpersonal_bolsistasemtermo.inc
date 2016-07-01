<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Lista de Bolsistas sem termo de compromisso</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

 


echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "select distinct replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
			   i.iusnome, 
			   i.iusemailprincipal,
			   uniabrev||' - '||unidsc as universidade, 
			   case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
				     when s.ocuid is not null then oc.ocunome 
				     when s.oatid is not null then oatnome end as curso,
			   case when i.iustermocompromisso is null then 'Termo não preenchido'
					when i.iusnaodesejosubstituirbolsa=true then 'Não deseja receber'
					else '-' end as tipo
		
		
from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd 
inner join sisfor.sisfor s on s.sifid = t.sifid 
inner join sisfor.mensario m on m.tpeid = t.tpeid 
inner join sisfor.mensarioavaliacoes ma on ma.menid = m.menid 
inner join sisfor.folhapagamentoprojeto fp on fp.sifid = s.sifid and m.fpbid = fp.fpbid 
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
inner join public.unidade u on u.unicod = s.unicod
inner join workflow.documento d on d.docid = fp.docid 
where (i.iustermocompromisso is null or i.iusnaodesejosubstituirbolsa=true) and mavatividadesrealizadas='A' and d.esdid=1207 
order by i.iusnome";

$cabecalho = array("CPF","Nome","Email","IES","Curso","Tipo");

if($_REQUEST['verxls']) {

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_xls_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_xls_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
	exit;
	
} else {

	$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');

}

//echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_bolsasformacao.inc&verxls=1\';"></p>';



echo '</td>';
echo '</tr>';
echo '</table>';

 
?>