<?php

    set_time_limit(0);

	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	$db = new cls_banco();

    $sql = "select *
            from seguranca.usuariopwd_temp
            where ususenhamd5 is null
            limit 1000";
    $dados = $db->carregar($sql);
    if(is_array($dados)){
        foreach ($dados as $count => $dado) {
            $senha = md5_decrypt_senha($dado['ususenha'], '');
            $db->executar("update seguranca.usuariopwd_temp set ususenhamd5 = md5('$senha') where usucpf = '{$dado['usucpf']}'");
            $db->commit();
        }
    }

echo date('d/m/Y H:i:s');
?>

<script type="text/javascript">
    window.location.href = window.location.href;
</script>