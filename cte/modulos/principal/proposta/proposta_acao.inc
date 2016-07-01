<?php

///echo "acao:".$_REQUEST['ctrid'];

$dadosProposta =  $db->carregar(" select pro.ppaid,pro.crtid,pro.ppadsc from cte.proposicaoacao pro
left outer join cte.criterio crt on crt.crtid=pro.crtid 
where pro.crtid='".$_REQUEST['ctrid']."'");

?>
<html>
<head>
<title></title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
function enviarDescricao(campo,descricao){

var dados = descricao;

opener.document.getElementById(campo).value = dados;
var fake = opener.document.getElementById(campo + '_fake');
if ( fake )
{
	fake.value = dados;
} 

window.close();

}

</script>
</head>
<body  LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" style="background-image: url( '/imagens/fundo.gif' ); background-repeat: repeat-y;">

<table width="100%" align="center" border="0" class="tabela" cellpadding="1" cellspacing="1" >
	<tbody>
		<tr>
           <td style="padding:1px; background-color:#fafafa; color:#404040; vertical-align: top;" >
				<form method="POST" name="formulario2">
					
					<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td class="SubTituloEsquerda"><strong>Selecione:</strong></td>
							<td class="SubTituloEsquerda"><strong>Descrição Proposta de Ação:</strong></td>
						</tr>
						<? for($i = 0; $i < count($dadosProposta);$i++ ){  ?>
						<tr>
							<td class="TextoEsquerda"><INPUT TYPE="RADIO" NAME="OPCAO" VALUE="<?=$dadosProposta[$i]['ppadsc'] ?>" onclick="enviarDescricao('acidescricao',this.value);"></td>
							<td class="TextoEsquerda"><?= $dadosProposta[$i]['ppadsc']; ?></td>
						</tr>
						<? } ?>
					</table>
				</form>	
			</td>
	     </tr>
	</tbody>
</table>

</body>
</html>