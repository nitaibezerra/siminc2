<?
$sql = "SELECT 
				lvaid,
				lvapatrimonio, 
			 	lvatitulo,
			 	lvaautor,
			 	lvaeditora,
			 	lvaedicao,
			 	lvadtinclusao,
			 	lvastatus,
			 	docid
			FROM  
				conjur.livroacervo
			WHERE 
				lvaid = ".$_REQUEST['lvaid'];
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);

$sql = "SELECT 
				to_char(lvedtemprestimo, 'DD/MM/YYYY') as lvedtemprestimo,
				to_char(lvedtprevdevolucao, 'DD/MM/YYYY') as lvedtprevdevolucao,
				advid
			FROM  
				conjur.livroemprestimo
			WHERE 
				lvaid = ".$_REQUEST['lvaid']."
			order by lveid DESC
			limit 1	";
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);

if($advid){
	$sql = "SELECT	ent.entnome
			 FROM entidade.entidade ent
			 inner join conjur.advogados adv on adv.entid = ent.entid
			 WHERE adv.advid = " . $advid;
	$nomeAdvogado = $db->pegaUm($sql);
}


?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
  </head>
<body>
<form name="formulario" id="formulario" method="post">
<input type="hidden" name="requisicao" value="pesquisarprojeto">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2" align="center">
			<b>RECIBO DE EMPRÉSTIMO DE LIVRO</b>
		</td>
	</tr>
 	<tr>
		<td width="30%" class="SubTituloDireita">Patrimônio:</td>
		<td><?=$lvapatrimonio?></td>
	</tr>
 	<tr>
		<td class="SubTituloDireita">Título:</td>
		<td><?=$lvatitulo?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Emprestado para:</td>
		<td><?=$nomeAdvogado?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data Emprétimo:</td>
		<td><?=$lvedtemprestimo?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data Devolução:</td>
		<td><?=$lvedtprevdevolucao?></td>
	</tr>
	<tr>
		<td colspan="2" align="center">
			<?
			if($lvedtemprestimo){
				$partes = explode("/", $lvedtemprestimo);
				$dia = $partes[0];
				$mes = $partes[1];
				$ano = $partes[2];
			}
			?>
			<br><br><br>
				BRASÍLIA, <?=$dia?> DE <?=mes_extenso($mes)?> DE <?=$ano?>.
			<br><br><br><br><br>
			______________________________________________________________
			<br> <?=$nomeAdvogado?>
		</td>
	</tr>
	<tr class="notprint">
		<td bgcolor="#CCCCCC" colspan="2" align="center">
			<input type="button" name="imprimir" value="Imprimir" style="cursor:pointer;" onclick="self.print()">  
		</td>
	</tr>
</table>
</form>
</body>
</html>