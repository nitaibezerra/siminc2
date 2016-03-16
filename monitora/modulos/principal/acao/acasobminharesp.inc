<?
//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
//$sql = "select acaid from usuarioresponsabilidade where usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and acaid is not null";
$sql = "select ur.acaid from monitora.usuarioresponsabilidade ur inner join monitora.acao a ON a.acaid = ur.acaid where a.acasnrap = false and usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and ur.acaid is not null";
$RS=$db->record_set($sql);
$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);			
		$acaid_r .= $res[0]."|,|";
		}
	}
if ($acaid_r) $acaid_r = substr($acaid_r,0,strlen($acaid_r)-3);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Ações Sob Minha Responsabilidade','Clique no código para ver detalhes.');?>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$_SESSION['usucpf']?>"> Lista de Ações Sob Minha Responsabilidade</td>
    </tr>
</thead>
    <tr>
      <td id="td<?=$_SESSION['usucpf']?>"></td>
    </tr>
</table>
<script language="JavaScript">
//Este script popula a página com as ações do programa
abreconteudo('geral/listaacao.php?acaid=<?=$acaid_r?>','<?=$_SESSION['usucpf']?>');
</script>