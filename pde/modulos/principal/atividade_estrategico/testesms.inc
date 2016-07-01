<?php

$micid = $_REQUEST['micid'];

include_once APPRAIZ . "includes/classes/Sms.class.inc";


if ($_POST['nredddcelular'] && is_array($_POST['nredddcelular'])) {
    $aCelularEnvio = array();
    foreach ($_POST['nredddcelular'] as $count => $nredddcelular) {

        if ($nredddcelular && $_POST['nrecelular'][$count] ) {
            $aCelularEnvio[] = '55' . $nredddcelular . str_replace('-', '', $_POST['nrecelular'][$count]);
        }
    }
    if($aCelularEnvio && $micid){

        $sql = "SELECT distinct
					a1.atiapelido,
					ind.indapelido,
					ent.entnome, '0' || ent.entnumdddcelular || ' ' ||ent.entnumcelular as celular_executor
				FROM pde.atividade a1
					INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
					INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
					INNER JOIN pde.monitoraitemchecklist mic on mic.atiid = a3.atiid AND mic.micstatus = 'A'
					INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
					INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
					INNER JOIN painel.indicador ind on mic.indid = ind.indid
					INNER JOIN workflow.documento doc ON doc.docid = i.docid
					INNER JOIN pde.monitorametaentidade me on me.mnmid = mnm.mnmid
					inner join entidade.entidade ent ON ent.entid = me.entid
				WHERE
				mic.micid = $micid
				and ent.entnumcelular is not null
				limit 1";
        $pendencia = $db->pegaLinha($sql);

        $conteudo = 'Hoje deverá ser informado, no Projeto "' . $pendencia['atiapelido'] . '", o item "' . $pendencia['indapelido'] . '". Responsável: ' . $pendencia['entnome'] . ' (' . $pendencia['celular_executor'] . ')';

        $sms = new Sms();
        $sms->enviarSms($aCelularEnvio, $conteudo);
    }
}

?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>Responsáveis pela cobrança</title>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		
		
		<script type="text/javascript">
			$(function(){
				$('#btn_salvar').click(function(){
					$('#vincular_responsavel').submit();
				});
			});
		</script>
		
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<?php if('000' == $codigo){ ?>
			<h3>Mensagem Enviada com Sucesso</h3>
		<?php } ?>
		<h2></h2>
		<form action="#" name="enviarsms" id="enviarsms" method="post">
			<input type="hidden" name="micid" value="<?php echo $micid; ?>" />
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;" colspan="4"><h4>Enviar SMS Teste</h4></td>
				</tr>		
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;">DDD</td>
					<td style="text-align: center;">Celular</td>
				</tr>	
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;"><?=campo_texto('nredddcelular[]', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $nredddcelular) ?></td>
					<td style="text-align: center;"><?=campo_texto('nrecelular[]', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', '', '', $nrecelular) ?></td>
				</tr>
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;"><?=campo_texto('nredddcelular[]', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $nredddcelular) ?></td>
					<td style="text-align: center;"><?=campo_texto('nrecelular[]', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', '', '', $nrecelular) ?></td>
				</tr>
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;"><?=campo_texto('nredddcelular[]', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $nredddcelular) ?></td>
					<td style="text-align: center;"><?=campo_texto('nrecelular[]', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', '', '', $nrecelular) ?></td>
				</tr>
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;"><?=campo_texto('nredddcelular[]', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $nredddcelular) ?></td>
					<td style="text-align: center;"><?=campo_texto('nrecelular[]', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', '', '', $nrecelular) ?></td>
				</tr>
				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
					<td style="text-align: center;"><?=campo_texto('nredddcelular[]', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $nredddcelular) ?></td>
					<td style="text-align: center;"><?=campo_texto('nrecelular[]', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', '', '', $nrecelular) ?></td>
				</tr>
			</table>
			<input type="submit" value="Enviar" id="btn_salvar">
		
		</form>
	</body>
</html>
