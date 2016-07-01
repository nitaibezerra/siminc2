<title>
SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<?php
		// monta cabeçalho
		//include APPRAIZ . 'includes/cabecalho.inc';
		$db->cria_aba( $abacod_tela, $url, '' );
		monta_titulo( $titulo_modulo, '&nbsp;' );
		?>
		<!-- biblioteca javascript local -->
		<script type="text/javascript">
</script>

<!-- corpo da página -->

<form action="" method="post" name="formulario">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">


<tr>
<td><b>Onde estava</b></td>
<td><b>O que aconteceu</b> </td>
<td><b>Quem fez</b></td>
<td><b>Quando fez</b> </td>
</tr>

<tr>
<td>Em elaboração</td>
<td>Agurdando aprovação </td>
<td>Luiz Fernando</td>
<td>29/08/2007 <br> 15:14:18 </td>
</tr>

<tr>
<td>Agurdando aprovação</td>
<td>Aprovado</td>
<td>Cristiano Cabral</td>
<td>30/08/2007 <br> 10:11:08 </td>
</tr>



</table>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">


<tr>
<td><b>Onde está agora:</b></td>
<td><b>Com quem está agora:</b> </td>
</tr>

<tr>
<td>Aguardando Avaliação</td>
<td>Cristiano Cabral </td>
</tr>


</table>
<p>&nbsp;</p>

<div>
<center><input class="botao" type="button" value="Fechar" onclick="window.close();"></center>

</div>

</form>