<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:prgsobminharesp.inc
   finalidade: listar os programas sob responsabilidade de quem está logado
   */
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?

$sql = "select p.prgid, p.orgcod from elabrev.ppaprograma_proposta p
inner join elabrev.ppaacao_proposta a on a.prgid=p.prgid 
inner join elabrev.unidade_acao ua on ua.acaid=a.acaid
inner join elabrev.usuarioresponsabilidade ur on ur.unicod=ua.unicod 
and ur.usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and ur.unicod is not null 
group by p.prgid, p.orgcod";

$RS=$db->record_set($sql);
$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);
		if (($res[1]==$_SESSION['ittorgao'] ) or $db->testa_uma())			
		$prgid_r .= $res[0]."|,|";

		}
	}
	
// busca responsabilidades no programa diretamente
$sql = "select p.prgid,p.orgcod from elabrev.ppaprograma_proposta p
inner join elabrev.usuarioresponsabilidade ur on ur.prgid=p.prgid and ur.usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and ur.prgid is not null";

$RS=$db->record_set($sql);

$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);
		if (($res[1]==$_SESSION['ittorgao'] ) or $db->testa_uma())			
		$prgid_r .= $res[0]."|,|";

		}
	}
	
$sql = "select p.prgid from elabrev.ppaprograma_proposta p
where prgstatus in ('A','N') and usucpf='".$_SESSION['usucpf']."'";
$RS=$db->record_set($sql);

$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);
		$prgid_r .= $res[0]."|,|";
		}
	}
	
if ($prgid_r) $prgid_r = substr($prgid_r,0,strlen($prgid_r)-3);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Programas sob minha Responsabilidade','Clique no programa para ver detalhes.');?>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$_SESSION['usucpf']?>"> Lista de Programas sob minha Responsabilidade</td>
    </tr>
</thead>
    <tr>
      <td id="td<?=$_SESSION['usucpf']?>"></td>
    </tr>
</table>
<script language="JavaScript">
//Este script popula a página com as ações do programa
  abreconteudo('geral/listaprograma.php?prgid=<?=$prgid_r?>','<?=$_SESSION['usucpf']?>');
 //abreconteudo('geral/listaprg.php?prgid=<?=$prgid_r?>','<?=$_SESSION['usucpf']?>');  
</script>