<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Cache-Control" content="no-cache">

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<table class=listagem width=100%>
    <tr>
        <td class=SubTituloCentro>Secretários Municipais Cadastrados</td>
    </tr>
</table>

<?php listarMunicipiosCadastrados( array( 'relatorio' => true ) ); ?>

<table class=listagem width=100%>
    <tr>
        <td class=SubTituloCentro>Secretários Estaduais Cadastrados</td>
    </tr>
</table>

<?php listarEstadosCadastrados( array( 'relatorio' => true ) ); ?>

