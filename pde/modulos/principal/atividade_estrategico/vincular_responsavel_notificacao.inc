<?php

include (APPRAIZ.'www/painel/_funcoes_metas.php');

$url = "$url&micid={$_POST['micid']}";

if ($_REQUEST['ajax'] == 'salvarcontato' && $_REQUEST['usucpf']) {

    $sql = "select 1 from pde.responsavelcelular where usucpf = '{$_REQUEST['usucpf']}'";
    $existeCelular = $db->pegaUm($sql);

    $campo = 2 == strlen($_REQUEST['numero']) ? 'dddcelular' : 'celular';

    ver($campo);

    if ($existeCelular) {
        $sql = "update pde.responsavelcelular set
                    $campo = '{$_REQUEST['numero']}'
                where usucpf = '{$_REQUEST['usucpf']}'";
    } else {
        $sql = "insert into pde.responsavelcelular (usucpf, $campo)
                                            values ('{$_REQUEST['usucpf']}', '{$_REQUEST['numero']}')";
    }
    $db->executar($sql);
    $db->commit();
    die;
}

if ($_POST['micid']) {
    try {
        $sql = "delete from pde.notificacaometaresponsavel where micid = {$_POST['micid']}";
        $db->executar($sql);
        if (is_array($_POST['usucpf'])) {
            foreach ($_POST['usucpf'] as $usucpf) {
                $sql = "insert into pde.notificacaometaresponsavel (micid, usucpf)
                                                             values('{$_POST['micid']}', '$usucpf')";
                $db->executar($sql);
            }
        }
        $db->commit();

        echo "<script>
            alert('Operação realizada com sucesso!');
            window.close();
        </script>";

        alertlocation(array(
            'alert'=>'Operação realizada com sucesso!',
            'location'=>$url
        ));
    } catch (Exception $e) {
        $db->Rollback();
        alertlocation(array(
            'alert'=>'Ocorreu um erro ao realizar operação!',
            'location'=>$url,
            'javascript'=>'window.close()'
        ));
    }
}

$sql = "SELECT DISTINCT
            usuario.usucpf,
            usuario.usunome,
            rec.dddcelular,
            rec.celular,
            nu.checado
        FROM seguranca.usuario usuario
            inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
            left join pde.responsavelcelular rec on rec.usucpf = usuario.usucpf
            left join (
                select usucpf, 1 as checado
                from pde.notificacaometaresponsavel nmr
                where nmr.micid = '{$_REQUEST['micid']}'
            ) nu on nu.usucpf = usuario.usucpf
        WHERE usuario.usunome is not null
        and usuariosistema.suscod = 'A'
        and usuariosistema.sisid = '132'
        ORDER BY  usuario.usunome";
$usuarios = $db->carregar($sql);

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
				jQuery('#btn_salvar').click(function(){
					$('#vincular_responsavel').submit();
				});

				jQuery('.atualiza_valor').change(function(){
					jQuery.ajax({
					    url: '<?php echo $url . "&ajax=salvarcontato&numero="; ?>' + jQuery(this).val() + '&usucpf=' + jQuery(this).attr('usucpf')
			    	});
				});
			});
		</script>

	</head>
    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff" onunload="javascript: window.opener.location.reload();">
        <form action="#" name="vincular_responsavel" id="vincular_responsavel" method="post">
            <input type="hidden" name="micid" id="micid" value="<?php echo $_REQUEST['micid']; ?>" />
        	<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
        		<tr bgcolor="#cdcdcd">
        			<td align="center" width="100%" colspan="4" height="25" ><label style="color: rgb(0, 0, 0); font-size:12px;" class="TituloTela">Responsáveis pela cobrança</label></td>
        		</tr>
        		<tr bgcolor="#dcdcdc">
        			<td width="1" style="text-align: center;">
        				&nbsp;
        			</td>
        			<td valign="top" align="center">Usuários</td>
        			<td valign="top" align="center">DDD</td>
        			<td valign="top" align="center">Celular</td>
        		</tr>
        		<?php
        		if ( $usuarios ):
        			$numLinha = 0;
        			foreach ($usuarios as $dado): ?>
        				<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
        					<td width="1" style="text-align: center;">
        						<input type="checkbox" name="usucpf[]" type="checkbox" value="<?=$dado['usucpf']?>" <?php echo $dado['checado'] ? 'checked="checked"' : ''; ?>/>
        					</td>
        					<td valign="top"><label><?=$dado['usunome'] ?></label></td>
                            <td style="text-align: center;"><?=campo_texto('dddcelular[' . $dado['usucpf'] . ']', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', 'class="atualiza_valor" usucpf="' . $dado['usucpf'] . '"', '', $dado['dddcelular']) ?></td>
                            <td style="text-align: center;"><?=campo_texto('celular[' . $dado['usucpf'] . ']', 'N', 'S', 'Celular', 8, 9, '####-####', '', '', '', '', 'class="atualiza_valor" usucpf="' . $dado['usucpf'] . '"', '', $dado['celular']) ?></td>
        				</tr>
        			<?php $numLinha++;
        			endforeach;
        		else: ?>
        			<tr>
        				<td align="center" colspan="2" style="color:#cc0000;">Não foram encontrados registros.</td>
        			</tr>
        		<?php endif; ?>
        	</table>

        	<input type="button" value="Vincular" name="Vincular" style="margin-left: 20px;" id="btn_salvar">

    	</form>
	</body>
</html>