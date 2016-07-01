<?php

if(count($_POST)){
	$obEmpenhoRAP = new LiquidacaoEmpenhoRap();
	$obEmpenhoRAP->enviaLiquidaEmpenhoRAP( $_POST );
}
// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Liquidação de Empenho Inscritos em RAP', '' );

$sql = "SELECT DISTINCT
			'<center><input type=\"checkbox\" name=\"chkLiquidar[' || ptr.ptrid || ']\"/></center>' as acoes,
		    ptr.ptrcod||'/'||ptr.ptrexercicio as pta, 
		    ptr.ptrnumprocessoempenho,
		    ptr.ptrnumconvenio||'/'||ptr.ptrexercicio as convenio,
		    sum(exf.exfvalor) as valorEmpenhado,
		    orb.orbvalorpagamento,
		    mun.mundescricao, 
		    ptr.ptrcod
		FROM
			emenda.planotrabalho ptr
		    inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
		    inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid and exf.pedid = pte.pedid
		    left join emenda.ordembancaria orb on orb.exfid = exf.exfid
		    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid and enbstatus = 'A'
            left join territorios.municipio mun on mun.muncod = enb.muncod
		WHERE 
			ptr.ptrstatus = 'A'
		    and exf.exfstatus = 'A'
		    and ptr.ptrexercicio = '{$_SESSION['exercicio']}'
		    /*and exf.exfnumempenhooriginal is not null and exf.exfnumempenhooriginal <> ''
		    and ( (select sum(exfvalor) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumempenhooriginal is not null) > orb.orbvalorpagamento or orb.orbnumordembancaria is null)*/
		GROUP BY
			ptr.ptrid,
		    ptr.ptrcod,
		    ptr.ptrnumprocessoempenho,
		    ptr.ptrnumconvenio,
		    ptr.ptrexercicio,
		    orb.orbvalorpagamento,
		    mun.mundescricao
		order by ptr.ptrcod";
//and ( (select sum(exfvalor) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumempenhooriginal is not null) > orb.orbvalorpagamento or orb.orbnumordembancaria is not null)
$arDados = $db->carregar( $sql );

$cabecalho = array("Ação", "Nº PTA", "Nº Processo", "Nº Convenio FNDE", "Valor Empenhado", "Valor Pago", "Município");
//echo $db->monta_lista($sql, $cabecalho, 100000, 4, 'N','Center', '', 'formconvenio');

$usuario = 'MECTIAGOT'; 
$senha   = 'M3135689';
?>
<form name="formconvenio" id="formconvenio" action="" enctype="multipart/form-data" method="post">
<table id="tblform" class="tabela" width="95%" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td class="SubTituloDireita">Usuário</td>
		<td><input type="text" class="normal" name="usuario" id="usuario" value="<?=$usuario; ?>" size="40" maxlength="15" title="Usuário"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Senha</td>
		<td><input type="password" class="normal" name="senha" id="senha" value="<?=$senha; ?>" size="40" maxlength="15" title="Senha"></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td align="left" colspan="2">
			<input type="button" value="Enviar RAP" name="btnConversao" id="btnConversao" onclick="enviarConvenioRAP();" style="cursor: pointer;">
		</td>
	</tr>
</table>
<table class="listagem" align="center" bgcolor="#f5f5f5" border="0" cellpadding="2" cellspacing="0" width="95%">
	<thead>
	<tr>
		<?
		foreach ($cabecalho as $cab) {
			echo '<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" align="Center" bgcolor="" 
					valign="top"><strong>'.$cab.'</strong></td>';
		}		
		?>
	</tr>
	</thead>
	<?
	if($arDados){
		foreach ($arDados as $key => $v) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			echo '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
					  <td align="left"><center>'.$v['acoes'].'</center></td>
					  <td align="left">'.$v['pta'].'</td>
					  <td style="color: rgb(0, 102, 204);" align="right">'.$v['ptrnumprocessoempenho'].'<br></td>
					  <td align="left">'.$v['convenio'].'</td>
					  <td style="color: rgb(0, 102, 204);" align="right">'.number_format($v['valorempenhado'], 2, ',', '.').'<br></td>
					  <td style="color: rgb(0, 102, 204);" align="right">'.number_format($v['orbvalorpagamento'], 2, ',', '.').'<br></td>			  
					  <td>'.$v['mundescricao'].'</td>			  
				  </tr>';
		}
		totalizadorRegistroLista(sizeof($arDados));
	} else{
		echo '<tr>
				  <td colspan="7"><table width="100%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
					</table>
				  </td>			  
			  </tr>';
	}
	?>
	</table>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript">	
	function enviarConvenioRAP(){
		if( document.getElementById('usuario').value == '' ){
			alert('O campo usuário e de preenchimento obrigatório');
			document.getElementById('usuario').focus();
			return false;
		}
		if( document.getElementById('senha').value == '' ){
			alert('O campo senha e de preenchimento obrigatório');
			document.getElementById('usuario').focus();
			return false;
		}
		 var inputs = $("#formconvenio :checkbox").serialize();
		 $("#formconvenio").submit();
	}
</script>