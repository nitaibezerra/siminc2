<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:acasobminharesp.inc
   finalidade: listar as ações sob responsabilidade de quem está logado
   */
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
/*
$sql = "select a.acaid, a.acasnpadronizada,a.acasnsetpadronizada from elabrev.ppaacao_proposta a
inner join elabrev.unidade_acao ua on ua.acaid=a.acaid
inner join elabrev.usuarioresponsabilidade ur on ur.unicod=ua.unicod 
and ur.usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and ur.unicod is not null";
 */
$sql = "select a.acaid, a.acasnpadronizada,a.acasnsetpadronizada from elabrev.ppaacao_proposta a
where a.usucpf='".$_SESSION['usucpf']."' ";

$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);
		
		if (($res[1]<>'t' and $res[2]<>'t') or $db->testa_uma())			
		$acaid_r .= $res[0]."|,|";

		}
	}


// busca responsabilidades na ação diretamente
$sql = "select a.acaid, a.acasnpadronizada,a.acasnsetpadronizada from elabrev.ppaacao_proposta a
inner join elabrev.usuarioresponsabilidade ur on ur.acaid=a.acaid and ur.usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and ur.acaid is not null";

$RS=$db->record_set($sql);
$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);
		if (($res[1]=='f' and $res[2]=='f') or $db->testa_uma())			
		$acaid_r .= $res[0]."|,|";

		}
	}

	
	
if ($acaid_r) $acaid_r = substr($acaid_r,0,strlen($acaid_r)-3);

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Ações sob minha Responsabilidade','Clique na ação para ver detalhes.');?>

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
  abreconteudo('geral/listaacao2.php?acaid=<?=$acaid_r?>','<?=$_SESSION['usucpf']?>');
</script>