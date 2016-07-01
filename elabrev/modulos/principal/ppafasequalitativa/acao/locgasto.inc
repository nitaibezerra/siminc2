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

function testa_localizador($reg,$acao)
{
	global $db;
	// esta função irá verificar se o localizsdfor já existe 
	// marcado na ação
	$sql = "select logstatus, usucpf from elabrev.localizadorgasto_acao where acaid=$acao and regcod='$reg'";
	$status=$db->pegalinha($sql);
	if ($status['logstatus']=='A')
		return ' checked disabled ';
	else if ($status['logstatus']=='N' and $status['usucpf']==$_SESSION['usucpf'])
		return ' checked enabled ';
	else if ($status['logstatus']=='N' and $status['usucpf']!=$_SESSION['usucpf'])
	    return ' checked disabled ';
	else return '';
}


if ($_REQUEST['act']=='enviar')
{
	$sql = "select regcod from regiao where regstatus='A'";
	$RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas >= 0) {
    	$sqlremocao="delete from elabrev.localizadorgasto_acao where usucpf='".$_SESSION['usucpf']."' and acaid=".$_SESSION['acaid'];
    	$db->executar($sqlremocao);
    	for ($i=0;$i<=$nlinhas;$i++)
    	{
    		$res =  @$db->carrega_registro($RS,$i);
    		foreach($res as $k=>$v) {${$k}=$v;};
    		if ($_REQUEST["$regcod"])
    		{    			
    			$sql= "insert into elabrev.localizadorgasto_acao (acaid,logstatus,usucpf,regcod,logjustificativa) values (".$_SESSION['acaid'].",'N','".$_SESSION['usucpf']."','$regcod','".$_REQUEST['logjustificativa']."')";
    			$db->executar($sql);
    		} 
    	$db->commit();
		}	
    }
   // $db->sucesso();
    $db->sucesso($modulo);
}

$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
    $res =  @$db->carrega_registro($RS,0);
    foreach($res as $k=>$v) {${$k}=$v;};
}

include APPRAIZ."includes/cabecalho.inc";		
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Localizadores de Gasto da Ação <br>(marque os localizadores que deseja incluir)';
monta_titulo($titulo_modulo,'');
$sql = "select logjustificativa from elabrev.localizadorgasto_acao where acaid=".$_SESSION['acaid']." and usucpf='".$_SESSION['usucpf']."' limit 1";
$logjustificativa = $db->pegaUm($sql);


$sql= "select a.prgcod, p.prgdsc, p.prgid, a.acaid, a.acacod, a.acadsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where a.prgid=p.prgid and a.acastatus='A' and a.prsano = '".$_SESSION['exercicio']."'  and a.prgid=".$_SESSION['prgid']."  and a.acaid =".$_REQUEST['acaid']." limit 1";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$acacod1 = $acacod;
$prgcod1 = $prgcod;
?>


<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <td align='right' class="SubTituloDireita"><b>Programa:</b></td>
    <td><?=campo_texto('prgcod1','N','','',5,5,'','');?><?=campo_texto('prgdsc','N','','',80,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>Código da ação:</b></td>
    <td><?=campo_texto('acacod1','N','','',5,5,'','');?>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>Título da ação:</b></td>
    <td><?=campo_texto('acadsc','N','N','',120,100,'','');?></td>
  </tr>
</table>

<form method="POST"  name="formulario">
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr><td>
<?
$sql = "select * from regiao where regstatus='A' and regtipo <>'E' order by regtipo,regdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$i=0;
while ($i<=$nlinhas)
{
    $res =  $db->carrega_registro($RS,$i);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    print "<input type='checkbox' name='$regcod' value='$regcod' ".testa_localizador($regcod,$_SESSION['acaid']).">$regdsc<br>";
    $i++;
} ?>
</td>
<td>
<?
$sql = "select * from regiao where regstatus ='A' and regtipo ='E' order by regtipo,regdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$i=0;
while ($i<=13)
{
    $res =  $db->carrega_registro($RS,$i);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    print "<input type='checkbox' name='$regcod' value='$regcod' ".testa_localizador($regcod,$_SESSION['acaid']).">$regdsc<br>";
    $i++;
} ?>
</td>
<td>
<?
$sql = "select * from regiao where regstatus='A' and regtipo ='E' order by regtipo,regdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$i=14;
while ($i<=$nlinhas)
{
    $res =  $db->carrega_registro($RS,$i);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    print "<input type='checkbox' name='$regcod' value='$regcod' ".testa_localizador($regcod,$_SESSION['acaid']).">$regdsc<br>";
    $i++;
} ?>
</td>

</tr>
<tr><td>Justificativa</td><td colspan="2"><?=campo_textarea('logjustificativa','N','S','',80,3,'','','','');?></td>
</tr>
<?if ($db->verifica_momento())
  {?>  
  <tr ><td colspan="3" align="center"><input type='button'  value='Gravar' onclick="gravar_janela('A')"></tr>      
  <?}?>
</table>

</form>  

    

<script>
  function gravar_janela()
  {
  	ok=0;
    if (document.formulario.logjustificativa.value =='')
    {
  	   if( window.confirm( "Você está gravando uma proposta sem justificativa. \n Este campo não é obrigatório. \n Caso queira justificar, cancele e volte à edição." ))
    	{
   	   	  	ok=1;
    	}
    	else
    	{
    	document.formulario.logjustificativa.focus();
    	return;
    	}
    }
    
  	if (ok==1)
  	{
  	  	document.formulario.act.value='enviar';
  	    document.formulario.submit();
    }
  }
 

</script>

