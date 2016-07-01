<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:gerencaca.inc
   Finalidade: permitir a construção de relatorio gerencial de ações
    */
$modulo=$_REQUEST['modulo'] ;//
unset($_SESSION['LINHA']);
unset($_SESSION['title']);
unset($_SESSION['TIT']);
unset($_SESSION['data']);
unset($_SESSION['LIN']);
$_SESSION['LIN']= array();
$_SESSION['TIT']=array();

if ($_REQUEST['relat']=='r')
{
  unset($sqlrel);
  //if ($_REQUEST['lista']=='L')
     include "monitora/modulos//relatorio/programa/listagerprg.inc";
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<div class="notprint">
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório Gerencial de Ação';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>
<script language="JavaScript" src="includes/calendario.js"></script>
<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >

<center>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b>Escolha Listagem ou Dados Gerais. Se escolher Dados Gerais, marque pelo menos um dos dados. <br>Escolha os itens desejados (Filtros). <br>Pressione o botão "Gerar Relatório"</b></td></tr>
<tr><td colspan=2><hr></td></tr>
<tr valign='top'><td ><input type="radio" value="L" name="lista" onclick='listagem()'><b>Listagem</b></td><td><input type="radio" value="G" name="lista" ><b>Dados gerais</b><br>
<input type='checkbox' name='dadobas' value='d'>Dados Básicos &nbsp;&nbsp;<input type='checkbox' name='avapar' value='avp'>Avaliação e Parecer&nbsp;&nbsp;<input type='checkbox' name='finevo' value='fin'>Financeiro Evolução&nbsp;&nbsp;<input type='checkbox' name='respro' value='res'>Restrições e Providências&nbsp;&nbsp;<input type='checkbox' name='paralt' value='pag'>Parecer da Alta Gestão&nbsp;&nbsp;</td></tr>
<tr><td colspan=2><hr></td></tr>
<tr> <td align='right' class="SubTituloDireita">Programa:</td>
        <td>	<?$sql = "select prgid as CODIGO,prgcod ||'-'||prgdsc as DESCRICAO from programa order by prgcod ";
	  $db->monta_combo("prgid",$sql,'S',"Todos os Programas",'','');
	?></td>
</tr>
<tr> <td align='right' class="SubTituloDireita">Orgão responsável:</td>
        <td>	<?$sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'S',"Todos os órgãos",'','');
	?><br><b>Exceto o MEC</b> <input type="checkbox" name="exceto" value='1' title='Filtrar todos os órgãos com exceção do MEC'></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Título (ou parte):</td>
    <td><?=campo_texto('prgdsc','N','S','',100,100,'','');?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Tipo:</td>
    <td><?$sql = "select tprcod as CODIGO,tprnome as DESCRICAO from tipoprograma order by tprnome ";
	  $db->monta_combo("tprcod",$sql,'S',"Todos os Tipos",'','');
	?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">Horizonte temporal:</td>
    <td><input type="radio" value="f" name="prgsntemporario"> Contínuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="t">Temporário</td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Com Restrições:</td>
    <td>Ainda sem solução <input type="radio" name="cbrp" value='N'> Solucionadas <input type="radio" name="cbrp" value='S'> Ambas <input type="radio" name="cbrp" value='A'></td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Com Parecer da Alta Gestão:</td>
    <td><input type="checkbox" name="cbpalt" value='REST'></td>
</tr>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td></td><td><b>Avaliação</b></td></tr>
<tr><td align="right" class="subtitulodireita">Situação da Avaliação:</td>
<td>
<?    $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
	  $db->monta_combo("tpscodav",$sql,'S',"Todas as situações",'','');
	  ?>
</td></tr>
<tr><td align="right" class="subtitulodireita">Cor da avaliação:</td>
<td><?
	$sql = "select corcod as CODIGO,corsignificado as desc , corimgav as imagem from cor where corstatus='A' order by corcod desc ";
	$RSradio = $db->record_set($sql);
	$nlinhasradio = $db->conta_linhas($RSradio);
	for ($j=0; $j<=$nlinhasradio;$j++)
	{
	    $res2 =  $db->carrega_registro($RSradio,$j);
	    if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
        print "<input type='radio' value='".$codigo."' name='corcodav'  title='".$desc."'>";
	    print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
   }
?>
</td></tr>

<tr><td></td><td><b>Parecer</b></td></tr>
<tr><td align="right" class="subtitulodireita">Situação do parecer:</td>
<td>
<?    $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
	  $db->monta_combo("tpscodpar",$sql,'S',"Todas as situações",'','');
	  ?>
</td></tr>
<tr><td align="right" class="subtitulodireita">Cor do parecer:</td>
<td><?
	$sql = "select corcod as CODIGO,corsignificado as desc , corimgpar as imagem from cor where corstatus='A' order by corcod desc ";
	$RSradio = $db->record_set($sql);
	$nlinhasradio = $db->conta_linhas($RSradio);
	for ($j=0; $j<=$nlinhasradio;$j++)
	{
	    $res2 =  $db->carrega_registro($RSradio,$j);
	    if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
        print "<input type='radio' value='".$codigo."' name='corcodpar'  title='".$desc."'>";
	    print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
   }
?>
</td></tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align="right" class="subtitulodireita">Periodo:</td>
<td>Início <?=campo_data('dataini', 'N','S','','S');?> &nbsp;&nbsp;Fim <?=campo_data('datafim', 'N','S','','S');?> (Obs: O Período só é considerado para efeito de monitoramento)

</td></tr>

<tr bgcolor="#CCCCCC">
   <td></td>   <td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>
</tr>
</table>
</center>
</div>
</form>
</div>
<?

if ($sqlrel){
//print $sqlrel;
  if ($_REQUEST['lista']=='L')
     include "monitora/modulos//relatorio/programa/montagerprg.inc";
  else
     include "monitora/modulos//relatorio/programa/montadadosprg.inc";
}
?>

<script>
function validar_relatorio()
{
	if (!validaRadio(document.formulario.lista,'Listagem ou Dados Gerais')) return;
	if (document.formulario.lista[1].checked)
    {
       if (! document.formulario.dadobas.checked && ! document.formulario.avapar.checked && ! document.formulario.finevo.checked && ! document.formulario.respro.checked && ! document.formulario.paralt.checked ) {alert ('Preencher pelo menos um dos Dados Gerais'); return;}
       }

    document.formulario.relat.value='r';
   	document.formulario.submit();
}
function alterar_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadprograma&acao=C&prgid="+cod;

}
function monitorar_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/programa/relmonitoraprg&acao=R&prgid="+cod;

}
function imprime_rel(cod)
{
    <?
       $browser = get_browser();
       $saiu = false;
       if (is_array($browser)){
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }}
       if ($saiu){?>
          if (cod == 1) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          };
          if (cod== 2) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenprg&acao=R";
          }
       <?} else { ?>
          if (cod == 1) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
        }
          if (cod== 2) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenprg&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
            }

      <? } ?>
}

</script>
