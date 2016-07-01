<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:locgasto.inc
   Finalidade: permitir o dos localizadores de gasto
   */
$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];

if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; 
else $_REQUEST['acaid'] = $_SESSION['acaid'];

/*
elabrev.dadofisico
(
  acaid int4,
  loccodppa char(5) NOT NULL,
  fisqtdeprevistoano1 numeric(9),
  fisqtdeprevistoano2 numeric(9),
  fisqtdeprevistoano3 numeric(9),
  fisqtdeprevistoano4 numeric(9),
  fisqtdeprevistoano5 numeric(9),
  fisqtdeprevistoano6 numeric(9),
  acacodppa char(5) NOT NULL,
  prgcod char(4) NOT NULL,
  prsano char(4),
  locppaid int4,
  fisobs text,
  fissnmetanaocumulativa bool,
  fisstatus char(1) DEFAULT 'A'::bpchar,
  usucpf char(11),
    fisid serial NOT NULL,
*/
$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
    $res =  @$db->carrega_registro($RS,0);
    foreach($res as $k=>$v) {${$k}=$v;};
}
// carrega os ano base
$sql = "select a.* from anobase a order by anbordem";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
	for ($i=0;$i<=$nlinhas;$i++)
	{
       $res =  @$db->carrega_registro($RS,$i);
       foreach($res as $k=>$v) {${$k}=$v;};
       ${'ano'.($i+1)}=$anbvalor;
	}
}

if ($_REQUEST['fisid'])
{
	$sql="select df.*,p.prodsc, u.unmdsc from elabrev.dadofisico df inner join elabrev.ppaacao a on a.acaid=df.acaid inner join produto p on p.procod=a.procod inner join unidademedida u on u.unmcod=a.unmcod where fisid=".$_REQUEST['fisid'];
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	foreach($res as $k=>$v) {${$k}=$v;};
	$sql= "select * from elabrev.totais_previstos_dadfis where acacod='$acacod'";
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	foreach($res as $k=>$v) {${$k}=$v;};
}
include APPRAIZ."includes/cabecalho.inc";		
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Dados Físicos da Ação';
monta_titulo($titulo_modulo,'');
?>
 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="fisid" value=0>

	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td ><b><?=$prgcod?></b> - <?=$prgdsc?></td>
      </tr>
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Ação:</td>
        <td ><b><?=$acacod?></b> - <?=$acadsc?></td>
      </tr>
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Produto / Unidade de Medida:</td>
        <td ><?=$prodsc?> / <?=$unmdsc?></td>
      </tr>
	 <tr align="left">
        <td colspan="2"><hr></td>
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Meta cumulativa?:</td>
        <td><input type="radio" name="fissnmetanaocumulativa" value="t" <?=($fissnmetanaocumulativa=='t'?"CHECKED":"")?>>  Não
                &nbsp;<input type="radio" name="fissnmetanaocumulativa" value="f" <?=($fissnmetanaocumulativa=='f'?"CHECKED":"")?>> Sim</td>
      </tr>
    <tr>
        <td></td>
    <td>
       <table>
	 <tr align="left">
        <td align="right"><b>Valores do Localizador</td>
        <td></td>
        <td align="left"><b>Valor total da Ação</td>        
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 1 (<?=$ano1?>):</td>
        <td><?=campo_texto('fisqtdeprevistoano1','N','S','',20,18,'############,##','','','dica','');?></td>
      <td><?=number_format($s_ano1,2,',', ' ')?></td>
      </tr>    
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 2 (<?=$ano2?>):</td>
        <td><?=campo_texto('fisqtdeprevistoano2','N','S','',20,18,'############,##','','','dica','');?></td>
      <td><?=number_format($s_ano2,2,',', ' ')?></td>     
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 3 (<?=$ano3?>):</td>
        <td><?=campo_texto('fisqtdeprevistoano3','N','S','',20,18,'############,##','','','dica','');?></td>
      <td><?=number_format($s_ano3,2,',', ' ')?></td>     
      </tr>
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 4 (<?=$ano4?>):</td>
        <td><?=campo_texto('fisqtdeprevistoano4','N','S','',20,18,'############,##','','','dica','');?></td>
      <td><?=number_format($s_ano4,2,',', ' ')?></td>        
      </tr>  
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 5 (<?=$ano5?>):</td>
        <td><?=campo_texto('fisqtdeprevistoano5','N','S','',20,18,'############,##','','','dica','');?></td>
      <td><?=number_format($s_ano5,2,',', ' ')?></td>        
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Previsão ano 6 (<?=$ano6?>):</td>
        <td align='left'><?=campo_texto('fisqtdeprevistoano6','N','S','',20,18,'############,##','','','dica','');?></td>
      <td align="left"><?=number_format($s_ano6,2,',', ' ')?></td>        
      </tr> 
 
       </table>
    </td>

    </tr>
      <tr align="left">
        <td align='left' class="SubTituloDireita">Observações:</td>
        
        <td ><?=campo_textarea('fisobs','S','S','',80,3,'','','','');?>    </td>
      </tr>  
    
</table>
</form>  
<?
// primeiro precisa achar os acaid da tabela ppaacao referentes a ação na tabela ppaacao_proposta
	$sql= "select acacodppa, acaid as codacao from elabrev.ppaacao where acacod='$acacod' and prgcod='$prgcod' and prsano='".$_SESSION['exercicio']."' order by acacodppa";

	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Localizadores de Gasto Associados a esta Ação</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Opções</strong></td>
      <td valign="top" class="title"><strong>Dados Físicos --  Total de (<?=$nlinhas+1?>) - Descrição do lacalizador - Região - Município - Unidade</strong></td>     
    </tr>
</thead>
<tbody>
<?

	if ($nlinhas >= 0) {
		for ($i=0; $i<=$nlinhas;$i++)
		{
  			$res = @$db->carrega_registro($RS,$i);
		// a linha abaixo transforma em variáveis todos os campos do array
  			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  			// com o acacodppa e o codacao encontro o dado fisico na tabela dadofisico
  			   
  			$sql = "select df.fisid,pl.locdsc,pl.locppaid,r.regdsc, case pl.muncod when '' then 'Sem município' else m.mundsc end as munic, u.unicod||'-'||u.unidsc as unid from elabrev.dadofisico df inner join elabrev.ppalocalizador pl on pl.acacodppa=df.acacodppa and df.acaid = pl.acaid inner join regiao r on r.regcod=pl.regcod left join municipio m on m.muncod=pl.muncod inner join elabrev.ppaacao a on a.acacodppa=pl.acacodppa and a.acaid=pl.acaid inner join unidade u on u.unicod=a.unicod  where pl.acacodppa='$acacodppa' and pl.acaid=$codacao order by pl.locdsc"; 
   			

  			$RS2 = $db->record_set($sql);
  			$res = @$db->carrega_registro($RS2,0);
  			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  			// aqui tenho os dados do ppalocalizador
  			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
  			print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Ver/Alterar dados do localizador.' onclick=".'"editar_dadofis('."'$fisid')".'"'."></td><td>$locdsc - $regdsc - $munic - $unid</td></tr>";
  			
		}
	}	

else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de localizadores de gasto</td></tr>';
}
?>
</tbody>
    </table>  
    

<script>
  function editar_dadofis(cod)
  {

  	document.formulario.fisid.value=cod;
  	document.formulario.submit();
  }
  function atualizamunicipio(cod)
  {

	 if (cod != '') {document.formulario.submit();};
    }  

</script>

