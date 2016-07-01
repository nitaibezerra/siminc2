<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Exportar cursista para SISFOR</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

echo '<p> UF : ';
$sql = "SELECT ufpid as codigo, estuf as descricao FROM escolaterra.ufparticipantes uf 
		WHERE cadastrosgb=TRUE
		ORDER BY estuf";
$db->monta_combo('ufpid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'ufpid', false, $_REQUEST['ufpid']);
echo '</p>';

echo '<p align="center"><input type="button" value="Gerar" onclick="window.location=\'escolaterra.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_exportarcursistassisfor.inc&ufpid=\'+document.getElementById(\'ufpid\').value;"></p>';

if($_REQUEST['ufpid']) :


	$cpf_coordenadorcurso = $db->pegaUm("select 
											usu.iuscpf
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
											inner join sisfor.identificacaousuario usu on usu.iuscpf = s.usucpf 
											inner join territorios.municipio m on m.muncod = usu.muncodatuacao 
											left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
											left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
											left join sisfor.outraatividade oat on oat.oatid = s.oatid 
											left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
											where sifstatus='A' and m.estuf in(select estuf from escolaterra.ufparticipantes where ufpid='".$_REQUEST['ufpid']."') and (cur.curid=".CUR_ESCOLATERRA_SISFOR." OR cur2.curid=".CUR_ESCOLATERRA_SISFOR.") and e.esdid=".ESD_PROJETO_VALIDADO_SISFOR);

	$sql = "(
			SELECT i.iuscpf||';'||i.iusemailprincipal||';'||m.estuf||';'||m.muncod||';{$cpf_coordenadorcurso}' as lin FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			WHERE i.iusstatus='A' AND t.pflcod=".PFL_PROFESSOR." AND i.ufpid='".$_REQUEST['ufpid']."'
			) UNION ALL (
			SELECT i.iuscpf||';'||i.iusemailprincipal||';'||m.estuf||';'||m.muncod||';{$cpf_coordenadorcurso}' as lin FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			WHERE i.iusstatus='A' AND t.pflcod=".PFL_TUTOR." AND i.ufpid='".$_REQUEST['ufpid']."'
			)
			";
	
	$cabecalho = array();
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
endif;
?>