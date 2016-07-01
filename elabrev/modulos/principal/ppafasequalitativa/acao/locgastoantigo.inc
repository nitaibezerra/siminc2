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

if ($_REQUEST['act']=='alterar')
{
	//dbg('vou alterar',1);
}
/*
elabrev.ppalocalizador
(
  loccodppa char(5) NOT NULL,
  muncod char(7),
  regcod char(2),
  locdsc varchar(255),
  locvlrcustototal float8,
  locvlrcustoateanoanterior float8,
  locqtdateanoanterior float8,
  locvlrcustoprevistoanocorrente float8,
  locqtdprevistoanocorrente float8,
  locrepercfinanceira text,
  locvlrrepercfinanceira float8,
  locmesinicio char(2),
  locanoinicio char(4),
  locmestermino char(2),
  locanotermino char(4),
  locsndscalterado bool,
  locobs text,
  locqtdcustototal float8,
  loccodreferenciasof int4,
  acacodppa char(5) NOT NULL,
  prgcod char(4) NOT NULL,
  loccod char(4),
  prsano char(4),
  acaid int4,
  locppaid serial NOT NULL,
*/
$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
    $res =  @$db->carrega_registro($RS,0);
    foreach($res as $k=>$v) {${$k}=$v;};
}
if ($_REQUEST['localizador'])
{
	$sql="select a.unicod,pl.*, a.taccod from elabrev.ppalocalizador pl inner join elabrev.ppaacao a on a.acacodppa=pl.acacodppa where locppaid=".md5_decrypt($_REQUEST['localizador'],'');
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	foreach($res as $k=>$v) {${$k}=$v;};
	$sql="select unicod  from elabrev.usuarioresponsabilidade ur where unicod='$unicod' and usucpf='".$_SESSION['usucpf']."'";
	$podeeditar=0;
	if ($db->testa_uma() or $db->pegaUm($sql)) $podeeditar=1;
}
include APPRAIZ."includes/cabecalho.inc";		
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Localizadores de Gasto da Ação';
monta_titulo($titulo_modulo,'');
?>
 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="localizador" value=0>
<input type=hidden name="act" value=0>


	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?></td>
      </tr>
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Ação:</td>
        <td><b><?=$acacod?></b> - <?=$acadsc?></td>
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Descrição:</td>
        <td><?=campo_texto('locdsc','N','S','',50,50,'','','','dica','');?></td>
      </tr>       
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Região:</td>
        <td><?
$sql= "select regcod as codigo, regdsc as descricao from regiao where regstatus='A' order by descricao";
$db->monta_combo('regcod',$sql,'S','Escolha a região','atualizamunicipio','','Escolha a região',400);
?></td>
      </tr>    
	<?
	if (($regcod !='NA') or ($_REQUEST['regcod'] and $_REQUEST["regcod"] !='NA') or (is_numeric($muncod))) {
	        ?>      
      <tr align="left">
        <td align='left' class="SubTituloDireita">Município:</td>
        <td><?
$sql= "select muncod as codigo, mundsc as descricao from municipio where munstatus='A' order by descricao";
$db->monta_combo('muncod',$sql,'S','Escolha o município','','','Escolha o município',400);
?></td>
      </tr> 
      <?}?>
   <? if ($taccod==1){?>
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Data de início e Fim:</td>
        <td>Início <?=campo_texto('locmesinicio','N','S','',3,2,'##','','','dica','');?>/<?=campo_texto('locanoinicio','N','S','',5,4,'####','','','dica','');?><br>
            Término <?=campo_texto('locmestermino','N','S','##',3,2,'','','','dica','');?>/<?=campo_texto('locanotermino','N','S','',5,4,'####','','','dica','');?>
        </td>
      </tr> 
      <?}?>      
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Custo total:</td>
        <td><?=campo_texto('locvlrcustototal','N','S','',20,18,'############.##','','','dica','');?>    </td>
      </tr> 
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Quantidade total:</td>
        <td><?=campo_texto('locqtdcustototal','N','S','',20,18,'############','','','dica','');?>    </td>
      </tr>   
      <tr align="left">
        <td align='left' class="SubTituloDireita">Custo total até o ano anterior:</td>
        <td><?=campo_texto('locvlrcustoateanoanterior','N','S','',20,18,'############.##','','','dica','');?>    </td>
      </tr> 
      <tr align="left">
        <td align='left' class="SubTituloDireita">Quantidade até ano anterior:</td>
        <td><?=campo_texto('locqtdateanoanterior','N','S','',20,18,'############','','','dica','');?>    </td>
      </tr> 
      <tr align="left">
        <td align='left' class="SubTituloDireita">Custo previsto no ano corrente:</td>
        <td><?=campo_texto('locvlrcustoprevistoanocorrente','N','S','',20,18,'############.##','','','dica','');?>    </td>
      </tr> 
      <tr align="left">
        <td align='left' class="SubTituloDireita">Quantidade prevista no ano corrente:</td>
        <td><?=campo_texto('locqtdprevistoanocorrente','N','S','',20,18,'#############','','','dica','');?>    </td>
      </tr>  
      <tr align="left">
        <td align='left' class="SubTituloDireita">Repercurssão financeira:</td>
        <td><?=campo_textarea('locrepercfinanceira','S','S','',80,3,'','','','');?>    </td>
      </tr>
      <tr align="left">
        <td align='left' class="SubTituloDireita">Valor da repercurssão financeira:</td>
        <td><?=campo_texto('locvlrrepercfinanceira','N','S','',20,18,'#############.##','','','dica','');?>    </td>
      </tr> 
      <tr align="left">
        <td align='left' class="SubTituloDireita">Observações:</td>
        <td><?=campo_textarea('locobs','S','S','',80,3,'','','','');?>    </td>
      </tr>  
      <? if ($podeeditar)  {?>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td><input type="button" name="btalterar" value="Alterar" onclick="altera('<?=$_REQUEST['localizador']?>')"></td>
      </tr> 
      <?}?>
      

    
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
      <td valign="top" class="title"><strong>Localizadores --  Total de (<?=$nlinhas+1?>) - Descrição do lacalizador - Região - Município - Unidade</strong></td>     
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
  			// com o acacodppa e o codacao encontro o localizador de gasto na tabela ppalocalizador
  			$sql = "select pl.locdsc,pl.locppaid,r.regdsc,u.unicod as unidade, case pl.muncod when '' then 'Sem município' else m.mundsc end as munic, u.unicod||'-'||u.unidsc as unid from elabrev.ppalocalizador pl inner join regiao r on r.regcod=pl.regcod left join municipio m on m.muncod=pl.muncod inner join elabrev.ppaacao a on a.acacodppa=pl.acacodppa and a.acaid=pl.acaid inner join unidade u on u.unicod=a.unicod  where pl.acacodppa='$acacodppa' and pl.acaid=$codacao order by pl.locdsc";
  			$RS2 = $db->record_set($sql);
  			$res = @$db->carrega_registro($RS2,0);
  			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  			// aqui tenho os dados do ppalocalizador
  			$sql="select unicod as unidade from elabrev.usuarioresponsabilidade ur where unicod='$unidade' and usucpf='".$_SESSION['usucpf']."'";
  			if ($db->pegaUm($sql)) $cor='red' ; else $cor='black';
  			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
  			print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Ver/Alterar dados do localizador.' onclick=".'"editar_localizador('."'".md5_encrypt($locppaid,'')."')".'"'."></td><td><font color='$cor'>$locdsc - $regdsc - $munic - $unid</font></td></tr>";
  			
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
  function editar_localizador(cod)
  {
  	document.formulario.localizador.value=cod;
  	document.formulario.submit();
  }
  function altera(cod)
  {
  	document.formulario.localizador.value=cod;
  	document.formulario.act.value='alterar';
  	document.formulario.submit();
  }  
  function atualizamunicipio(cod)
  {

	 if (cod != '') {document.formulario.submit();};
    }  

</script>

