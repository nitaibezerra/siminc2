<?php

cte_verificaSessao();

include APPRAIZ . 'includes/entidades.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo( $titulo_modulo, '' );

$inuid = (integer) $_SESSION["inuid"];
$entid = (integer) $_REQUEST["entid"];

?>
<script>
	
	function gravarEscola()
	{
		formulario = document.formulario;
		if ( !validaBranco( formulario.entnome, 'Nome' ) )
		{
			return;
		}
		formulario.submit();
	}
	
	function voltar()
	{
		window.location.href = '?modulo=principal/escolas&acao=A';
	}
	
</script>
<form method="post" action="" name="formulario">
	<input type="hidden" name="gravar" value="1" />
	<input type="hidden" name="entid" value="<?= $entid ?>" />
	<?php
	ent_exibir( $entid );
	?>
	<p align="center">
		<input
			type="button"
			name="Voltar"
			value="Voltar"
			align="right"
			onclick="voltar();"
		/>
	</p>
</form>

		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
