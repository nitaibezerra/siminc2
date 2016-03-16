<?php 
$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

if( $_REQUEST['obrid'] ){
	
	$endereco = $_REQUEST['endlog'].' - '.$_REQUEST['endnum'].', '.$_REQUEST['endcom'].', '.$_REQUEST['endbai'];
?>
<table>
	<tr>
		<td>
			Nome da Obra: 
		</td>
		<td>
			<?=$_REQUEST['obrdesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Programa/Fonte:
		</td>
		<td>
			<?=$_REQUEST['plititulo'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Tipo de Obra:
		</td>
		<td>
			<?=$_REQUEST['tobadesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Valor da Obra:
		</td>
		<td>
			<?=number_format($_REQUEST['obrvalorprevisto'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td>	
			Endereço completo:
			</br>CEP:
		</td>
		<td>
			<?=$endereco ?></br> <?=$_REQUEST['endcep'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Situação:
		</td>
		<td>
			<?=$_REQUEST['situacao'] ?>
		</td>
	</tr>
</table>

<?php 
	exit;
}

monta_titulo( 'Obras no Estado', $obraDescricao  );

echo cabecalho();

if($_SESSION['brasilpro']['estuf']){

	$sql = "SELECT DISTINCT
					'-' as acao, 
					pre.predescricao,
					pto.ptodescricao,					
					mun.mundescricao,
					pre.estuf,
					esd.esddsc,					
					usu.usunome,
					to_char(hstu.htddata,'DD/MM/YYYY HH24:MI:SS') as htddata,					
					(select usunome from (select distinct
								max(hd1.htddata) as data,
								us1.usunome,
								hd1.docid,
								ed1.esddsc
							from workflow.historicodocumento hd1
								inner join workflow.acaoestadodoc ac1 on
									ac1.aedid = hd1.aedid
								inner join workflow.estadodocumento ed1 on
									ed1.esdid = ac1.esdidorigem
								inner join seguranca.usuario us1 on
									us1.usucpf = hd1.usucpf
								left join workflow.comentariodocumento cd1 on
									cd1.hstid = hd1.hstid
							where
								ac1.esdiddestino in (210,211,212)
							and 
								hd1.docid = pre.docid
							group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata 
							order by data desc limit 1) as foo) as nomeanalista,
					resnumero
				FROM obras.preobra pre
				INNER JOIN cte.subacaoobra 				 sbo ON sbo.preid   = pre.preid
 				LEFT  JOIN territorios.municipio 		 mun ON pre.muncod  = mun.muncod
				LEFT  JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
				LEFT  JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
				LEFT  JOIN workflow.documento 			 doc ON doc.docid   = pre.docid 
				LEFT  JOIN workflow.estadodocumento 	 esd ON esd.esdid   = doc.esdid				
				LEFT  JOIN obras.pretipoobra 			 pto ON pre.ptoid   = pto.ptoid	
				LEFT  JOIN par.resolucao				 res ON res.resid   = pre.resid			
				LEFT JOIN (SELECT h.docid, max(h.hstid) hstid FROM workflow.historicodocumento h GROUP BY docid) hst ON doc.docid=hst.docid
				LEFT JOIN workflow.historicodocumento hstu ON hstu.hstid=hst.hstid
				LEFT JOIN seguranca.usuario usu ON hstu.usucpf=usu.usucpf
				WHERE 
					pre.prestatus = 'A' 
					AND pre.estuf = '".$_SESSION['brasilpro']['estuf']."'
					AND pre.tooid = ".OBRAS_ORIGEM_BRASILPRO."
					AND pre.preid not in (".$_REQUEST['preid'].")
				ORDER BY htddata DESC";
//	ver($sql);
	$arrObras = $db->carregar($sql);
}
?>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
	<?php if(!$_SESSION['brasilpro']['estuf'] || !$arrObras){ ?>
		<tr>
			<td align="center">
			Não existem obras no Estado.
			</td>
		</tr>
	<?php }else{ ?>
		<tr>
			<td align="center">
			<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="color: rgb(51, 51, 51);">
			<thead>
				<tr>
					<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">Nome da Obra
					</td>
				</tr> 
			</thead>
			</table>
			<?php 
				$arCabecalho = array("Ações","Nome da obra","Tipo da obra","Município","UF","Situação","Usuário", "Data da Situação", "Analista", "Resolução"); 
				$db->monta_lista_array($arrObras, $arCabecalho, 20, 10, 'N', '');
			?>
			</td>
		</tr>
	<?php } ?>
</table>
<script>

function redireciona( obrid ){

	window.opener.location = '/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid;
	return false;
}
</script>

