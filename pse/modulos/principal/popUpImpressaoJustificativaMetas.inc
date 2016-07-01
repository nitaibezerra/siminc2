<img src="/imagens/ico_print.jpg" style="cursor: pointer;width:40px;margin-left:15px;:center" border="0" title="Imprimir" class="imprimir">
<?
function recuperaEscolasJusid( $jusid, $motid ){

	global $db;

	$sql = "SELECT DISTINCT
				ent.entid,
				ent.entnome,
				coalesce(mujtotaleduescola,coalesce(pamquantedutotal,pacquantedutotal)) as escola,
				coalesce(mujtotaledupactuado,coalesce(pamquanteduseratend,pacquanteduseratend)) as pactuados,
				mujquantedureduzido as reducao
			FROM
				pse.justificativa jus 
			INNER JOIN pse.motivojus muj ON muj.jusid = jus.jusid
			INNER JOIN entidade.entidade ent ON ent.entid = muj.entid
			LEFT  JOIN pse.planoacaoc1 pam ON pam.entid = ent.entid
			LEFT  JOIN pse.planoacaoc2 pac ON pac.entid = ent.entid
			WHERE
				jus.jusid = $jusid AND muj.motid = $motid
			ORDER BY 
				1";

	return $db->carregar($sql);
}

if( $_REQUEST['jusid'] == '' ){
	echo "<script>window.close();</script>";
	die();
}

$sql = "SELECT DISTINCT
			con.entid,
			jus.jusid,
			jus.conid,
			jusoutromotivo,
			jusstatusgeratermo,
			-- educacao
			to_char(jussecnpj::numeric,'00,999,999/9999-99') as secnpj,
			jussenomesecretario as senomesecretario,
			jusseestadocivil as seestadocivil,
			jusseidentidade as seidentidade,
			jusseorgaoexp as seorgaoexp,
			to_char(jussecpf::numeric,'000,999,999-99') as secpf,
			--saude
			to_char(jussscnpj::numeric,'00,999,999/9999-99') as sscnpj,
			jusssnomesecretario as ssnomesecretario,
			jusssestadocivil as ssestadocivil,
			jusssidentidade as ssidentidade,
			jusssorgaoexp as ssorgaoexp,
			to_char(jussscpf::numeric,'000,999,999-99')  as sscpf,
			--motivos gravados
			mt1.motid as mot1,
			mt2.motid as mot2,
			mt3.motid as mot3,
			jusoutromotivo
		FROM
			pse.justificativa jus
		INNER JOIN pse.contratualizacao con ON con.conid = jus.conid
		LEFT  JOIN pse.motivojus 		mt1 ON mt1.jusid = jus.jusid AND mt1.motid = 1
		LEFT  JOIN pse.motivojus 		mt2 ON mt2.jusid = jus.jusid AND mt2.motid = 2
		LEFT  JOIN pse.motivojus 		mt3 ON mt3.jusid = jus.jusid AND mt3.motid = 3
		WHERE
			jus.jusid = ".$_REQUEST['jusid'];
$info = $db->pegaLinha($sql);

$sql = "SELECT
			ent.entid as entid,
			est.estdescricao as estado,
			mun.mundescricao as municipio,
			entnome as secretaria
		FROM
			pse.usuarioresponsabilidade urs
		INNER JOIN entidade.endereco ende ON ende.muncod = urs.muncod
		INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
		INNER JOIN territorios.estado est ON est.estuf = ende.estuf
		INNER JOIN entidade.entidade ent ON ent.entid = ende.entid
		INNER JOIN entidade.funcaoentidade fun ON fun.entid = ent.entid
		WHERE
			ent.entid = ".$info['entid'];
$secretaria = $db->pegaLinha($sql);

echo '<br />';
$titulo = "Justificativa Para o Não Alcance de Metas";
monta_titulo( $titulo, $portaria );
?>
<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top" width="15%"><b>UF:</b></td>
		<td><?=$secretaria['estado'] ?> </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio"><?=$secretaria['municipio'] ?> </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio"><?=$secretaria['secretaria'] ?> </td>
	</tr>
</table>
<?php 
monta_titulo('', '<div style="font-size:12;"><b>Informações Cadastrais das Secretarias Municipais da Saúde e Educação</b></div>');

if( $infoJust['jusid'] != '' ){
	$info = $infoJust;
}

$jussecnpj = $info['secnpj'];
$jussenomesecretario = $info['senomesecretario'];
$jusseestadocivil = $info['seestadocivil'];
$jusseidentidade = $info['seidentidade'];
$jusseorgaoexp = $info['seorgaoexp'];
$jussecpf = $info['secpf'];

$jussscnpj = $info['sscnpj'];
$jusssnomesecretario = $info['ssnomesecretario'];
$jusssestadocivil = $info['ssestadocivil'];
$jusssidentidade = $info['ssidentidade'];
$jusssorgaoexp = $info['ssorgaoexp'];
$jussscpf = $info['sscpf'];

$jusoutromotivo = $info['jusoutromotivo'];

$bloq = false;
$habil = 'S';
if( $info['jusstatusgeratermo'] == 'S' ){
	$bloq = true;
	$habil = 'N';
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script>
	$(document).ready(function(){
	
		window.print();

		$('.imprimir').click(function(){
			window.print();
		});
	});
	</script>
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td bgcolor="#c0c0c0" width="20%"></td>
			<td align="left" bgcolor="#c0c0c0" style="font-size:14;"><b>Secretaria de Saúde</b></td>
			<td align="left" bgcolor="#c0c0c0" style="font-size:14;"><b>Secretaria de Educação</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">C.N.P.J. da Secretaria</td>
			<td><?=str_replace(',','.',$jussscnpj ); ?> </td>
			<td><?=str_replace(',','.',$jussecnpj ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Nome completo do Secretário</td>
			<td><?=$jusssnomesecretario; ?> </td>
			<td><?=$jussenomesecretario; ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Estado civil do Secretário</td>
			<td><?=$jusssestadocivil; ?> </td>
			<td><?=$jusseestadocivil; ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Número da Carteira de Identidade</td>
			<td><?=$jusssidentidade; ?> </td>
			<td><?=$jusseidentidade; ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Orgão Expedidor</td>
			<td><?=$jusssorgaoexp; ?> </td>
			<td><?=$jusseorgaoexp; ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">CPF</td>
			<td><?=str_replace(',','.',$jussscpf ); ?> </td>
			<td><?=str_replace(',','.',$jussecpf ); ?> </td>
		</tr>
	</table>
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<?php 
		$sql = "SELECT DISTINCT mot.motid, mot.motdescricao 
				FROM pse.motivo mot
				INNER JOIN pse.motivojus mtj ON mtj.motid = mot.motid
				WHERE
					jusid = ".$_REQUEST['jusid'];
		$motivos = $db->carregar($sql);
		if( is_array($motivos) ){
			foreach( $motivos as $motivo ){
			?>
			<tr>
				<td class="SubTituloDireita">
					<div style="float:left;vertical-align:middle;line-height:25px;" >
						 <?=$motivo['motdescricao'] ?>
					</div>
				</td>
			</tr>
			<?php 
				if( $bloq ){
					$escolas = recuperaEscolasJusid( $info['jusid'], $motivo['motid'] );
				}else{
					$escolas = recuperaEscolasConid( $info['conid'], $motivo['motid'] );
				}
				if( is_array($escolas) ){
					foreach( $escolas as $k=>$escola ){
						if( $k%2 == 0 ){ $cor = 'bgcolor="white"'; }else{ $cor = ''; }
			?>
			<tr style="vertical-align:middle;line-height:25px;" <?=$cor ?>>
				<td>
					<?=$escola['entnome'] ?>
					- <b>Educandos da Escola:</b> <?=$escola['escola'] ?> 
					
					- <b>Educandos Pactuados:</b> <?=$escola['pactuados'] ?> 
					<?php
							if( $motivo['motid'] == 1 ){
					?>
					- <b>Redução de Educandos:</b>
					<?=$escola['reducao'] ?> 
					<?php 
							} 	
					?>
					<br>
				</td>
			</tr>
			<?php 	
					}
				}
			}
		}
		if( $info['jusoutromotivo'] != '' ){
		?>
		<tr>
			<td class="SubTituloDireita"> <div style="float:left;vertical-align:middle;line-height:25px;">Outros </div></td>
		</tr>
		<tr>
			<td>
				<?=$jusoutromotivo; ?>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
</body>