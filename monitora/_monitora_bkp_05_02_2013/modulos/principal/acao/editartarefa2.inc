<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:editapje.inc
   Finalidade: permitir a edição de acompanhamento físico
   */

$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['ptoid'])
{
	$ptoid = $_REQUEST['ptoid'];
	$sql = "select acaid from monitora.planotrabalho where ptoid=$ptoid";
	$acaid = $db->pegaum($sql);
	$parametros = "&ptoid=$ptoid";
	
	$sql = "select ptoorigemppa from monitora.planotrabalho where ptoid=$ptoid";
	$ppa = $db->pegaum($sql);
}
if ($ppa=='f') $ppa = 0;
if (! $_SESSION['acaid'] and ! $acaid) 
{
	?>
	<script>alert('Escolha primeiro a Ação!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

if ($_SESSION['acaid']) $acaid=$_SESSION['acaid'];
else $_SESSION['acaid']= $acaid;

  $autorizado = false;
  $coordpje=false;
  $digit=false;
  if ($db->testa_coordenador($acaid,'A')) {
  	$coordacao = true;
  	$status='coordpje';
  	$autorizado = true;
  }
   // verifica se é digitador
  if ($db->testa_digitador($acaid,'E')) {
  	$digit = true;
  	$status='digit';
  	  	$autorizado = true;
  }
// verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordacao = true; 
  	  $status='coordpje'; 
  	$autorizado = true;  	  
  }     

if (! $autorizado)
{
	   ?>
	      <script>
	         alert ('Acesso Negado!');
             window.close();
	      </script>
	   <?
	     exit();
}
if ($_REQUEST['act2']=='altera' or $_REQUEST['act2']=='aprova' or $_REQUEST['act2']=='desaprova')
{
	if (! $_REQUEST['expfinanceiro']) $_REQUEST['expfinanceiro']=0;
    if (! $_REQUEST['expobs']) $_REQUEST['expobs'] = '';
    $sql = "update monitora.execucaopto set expfinanceiro=".$_REQUEST['expfinanceiro'].", exprealizado=".$_REQUEST['exprealizado'].",usucpf='".$_SESSION['usucpf']."',expobs='".$_REQUEST['expobs']."',tpscod=".$_REQUEST['tpscod']." ,expdata='".$_REQUEST['expdata']."' where expid= ".$_REQUEST['expid'];
    if ($_REQUEST['act2']=='aprova')
    $sql = "update monitora.execucaopto set expsnaprovado='t',expfinanceiro=".$_REQUEST['expfinanceiro'].", exprealizado=".$_REQUEST['exprealizado'].",usucpf='".$_SESSION['usucpf']."',expobs='".$_REQUEST['expobs']."',tpscod=".$_REQUEST['tpscod']." ,expdata='".$_REQUEST['expdata']."' where expid= ".$_REQUEST['expid'];
   if ($_REQUEST['act2']=='desaprova')
    $sql = "update monitora.execucaopto set expsnaprovado='f',expfinanceiro=".$_REQUEST['expfinanceiro'].", exprealizado=".$_REQUEST['exprealizado'].",usucpf='".$_SESSION['usucpf']."',expobs='".$_REQUEST['expobs']."',tpscod=".$_REQUEST['tpscod']." ,expdata='".$_REQUEST['expdata']."' where expid= ".$_REQUEST['expid'];    
        $res = $db->executar($sql);
        $db->commit(); 
        $db->sucesso($modulo,$parametros);
}

if ($_REQUEST['act2']=='insere')
{
	if (! $_REQUEST['expfinanceiro']) $_REQUEST['expfinanceiro']=0;
    if (! $_REQUEST['expobs']) $_REQUEST['expobs'] = '';
    $sql = "select acaid from monitora.planotrabalho where ptoid=$ptoid";

    $acaid = $db->pegaUm($sql);

    if ($acaid)
      $sql = "insert into monitora.execucaopto (ptoid,exprealizado,usucpf,expobs,tpscod,expfinanceiro,acaid,expdata) values (".$_REQUEST['ptoid'].",".$_REQUEST['exprealizado'].",'".$_SESSION['usucpf']."','".$_REQUEST['expobs']."',".$_REQUEST['tpscod'].",".$_REQUEST['expfinanceiro'].",".$acaid.",'".$_REQUEST['expdata']."')";
      else
  $sql = "insert into monitora.execucaopto (ptoid,exprealizado,usucpf,expobs,tpscod,expfinanceiro,acaid,expdata) values (".$_REQUEST['ptoid'].",".$_REQUEST['exprealizado'].",'".$_SESSION['usucpf']."','".$_REQUEST['expobs']."',".$_REQUEST['tpscod'].",".$_REQUEST['expfinanceiro'].",".$acaid.",'".$_REQUEST['expdata']."')";
//dbg($sql,1);
    $res = $db->executar($sql); 

    $db->commit();
    if ($_REQUEST['act']=='I') {
   ?>
	      <script>
	         alert ('Transação realizada com sucesso!');
             opener.location.reload();
             window.close();
	      </script>
  <?
	     exit();
    }
    else $db->sucesso($modulo,$parametros);
                
}

$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

$ptoid = $_REQUEST['ptoid'];
$expid = $_REQUEST['expid'];

$sql= "select p.ptocod,p.ptodsc,p.ptoprevistoexercicio,ptotipo,ptoid_pai from monitora.planotrabalho p  where ptoid=$ptoid";
$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$atividade='Tarefa:';

$sql= "select p.ptodsc,p.ptoprevistoexercicio, p.ptodata_ini,p.ptodata_fim,u.unmdsc from monitora.planotrabalho p, unidademedida u where u.unmcod=p.unmcod and p.ptoid =".$ptoid;
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);

if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['act']=='editar')
{
	$sql= "select ep.* from monitora.execucaopto ep where ep.expid=$expid";
	$RS = $db->record_set($sql);

	$res =  $db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

}

if ($ptoprevistoexercicio<>0)
{
	
  $sql= "select sum(exprealizado) as realizado from monitora.execucaopto  where ptoid =".$ptoid;
  $realizado=$db->pegaum($sql);
  $desempenho=number_format(($realizado/$ptoprevistoexercicio)*100,0);
  if (! $desempenho) $desempenho='0';

}


if ($expid)
{
$sql= "select * from monitora.execucaopto  where expid=$expid";
$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;



}
if (! $expdata) $expdata = date('Ymd');


?>

<title>Administrar Acompanhamento de Tarefas</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<div align="center">
<table  width='95%' border="0" cellspacing="0" cellpadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Administrar Acompanhamento de Tarefas</label></td></tr>
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=<?=$_REQUEST['acao']?>>
<input type=hidden name="ptoid" value="<?=$ptoid?>">
<input type='hidden' name='vlrfin' value="<?=$vlrano-$gasto?>">
<input type='hidden' name='expid' value="<?=$expid?>">
<input type=hidden name="act2" value=''>
<input type='hidden' name='ptodata_ini' value="<?=$ptodata_ini?>">
<input type='hidden' name='ptodata_fim' value="<?=$ptodata_fim?>">

<?
if ($nlinhas >=0)
{
    $res =  $db->carrega_registro($RS,0);
    print '<input type=hidden name="alteracao" value=1>';
}
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
	<tr>
     <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;"><?=$atividade?></td><td><?=mostracod($ptoid).'-'.$ptodsc?></td>
      </tr>
	<tr>
     <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Datas de Início e Término:</td><td><?=formata_data($ptodata_ini).' a '.formata_data($ptodata_fim)?></td>
      </tr>

	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Previsto no Exercício:</td><td><?=$ptoprevistoexercicio?></td>
      </tr>
      <?if ($desempenho or $desempenho=='0'){?>
      	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Realizado até o momento:</td><td><?=$realizado .'  ('.$desempenho.' %)'?></td>
      </tr>
      <?}?>
	<tr>
	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Financeiro:</td><td><?= "<b>Total do projeto: R$ ".number_format($vlrano,2,',','.').
                   "<b><br>Gasto: R$ ".number_format($gasto,2,',','.').
                   "<b><br>Saldo: R$ ".number_format($vlrano-$gasto,2,',','.');?>
        </td>
      </tr>
	  	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Unidade de Medida:</td><td><?=$unmdsc?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data:</td>
        <td>
		<?=campo_data('expdata', 'S','S','','S');?>
	</td>
      </tr>      
	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Realizado<br>Físico:</td><td><?=campo_texto('exprealizado','S',$habil,'',10,8,'###########','');?></td>
      </tr>
      <? if ($ptotipo=='P') {?>
      
	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Gasto<br>Financeiro:</td><td><?=campo_texto('expfinanceiro','S',$habil,'',12,10,'###########','');?> (Valores em R$)</td>
      </tr>
      <?} else { print '<input type="hidden" name="expfinanceiro" value="0">';}?>
	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Situação:</td><td>
<?
	  $sql = "select tpscod as CODIGO,tpsdsc as DESCRICAO from tiposituacao where tpsstatus='A'  and tpscod<10 order by tpsdsc ";
	  $db->monta_combo("tpscod",$sql,'S',"Selecione a situação",'','');
?>
       </td>
     </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Observações:</td><td><?=campo_textarea("expobs",'N',"S",'',85,7,'')?></td>
    </tr>
    <tr bgcolor="#C0C0C0">
       <td colspan=2>
       <? if (! $_REQUEST['expid'] and ! $ppa) {?>
       <input type="button" class="botao" name="btgravar" value="Inserir" onclick="validar_cadastro('I')">
       <?} else if ( ! $ppa){?>
       <input type="button" class="botao" name="btgravar" value="Gravar" onclick="validar_cadastro('A')">       
       <?}
       if ($coordacao and $_REQUEST['expid']  and ! $ppa) {
       	  if ($expsnaprovado=='f') {
       	?>
        <input type="button" class="botao" name="btaprova" value="Aprovar o acompanhamento" onclick="validar_cadastro('C')">      
       
       <?} else {?>
        <input type="button" class="botao" name="btdesaprova" value="Retornar o acompanhamento para edição" onclick="validar_cadastro('R')">      
       
       <?}}?>
       <input type="reset" class="botao" value="Limpar" ><input type='button' class="botao" value='Fechar Janela' onclick="fechar_janela()"></td>
    </tr>
</table>
<?
    if ($_REQUEST['acao']=='A') {
  	$sql = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Editar Acompanhamento.\" onclick=\"editaracomp('||ep.expid||')\">' as acao,
to_char(ep.expdata,'dd/mm/YYYY'),ts.tpsdsc,ep.exprealizado as totalfisico from monitora.execucaopto ep inner join tiposituacao ts on ts.tpscod=ep.tpscod
inner join monitora.planotrabalho pt on pt.ptoid=ep.ptoid and pt.ptoid = $ptoid
inner join unidademedida um on um.unmcod=pt.unmcod 
order by ep.expdata desc";	
	$cabecalho = array( 'Editar', 'Data','Situação','Físico' );
	$db->monta_lista_simples( $sql, $cabecalho, 50, 20, '', '' ,'' );	
    }

?>
</form>
<script>
function editaracomp(cod)
{
   document.formulario.act.value = 'editar';
   document.formulario.expid.value = cod;  
   document.formulario.submit();
}
  function fechar_janela()
  {
    window.close();
    opener.location.reload();
  }
  function validar_cadastro(cod)
  {
  	// checa as datas
	if (!validaDataMaior(document.formulario.ptodata_ini , document.formulario.expdata))
	{
		alert("Data não pode ser anterior que Data Início da tarefa.");
		document.formulario.expdata.focus();
		return;
	}
	
	if (!validaDataMaior(document.formulario.expdata , document.formulario.ptodata_fim))
	{
		alert("Data não pode ser posterior que Data Término da tarefa.");
		document.formulario.expdata.focus();
		return;
	}	

    if (!validaBranco(document.formulario.exprealizado, 'Realizado')) return;

    if (!validaBranco(document.formulario.tpscod, 'Situação')) return;

    if (desformata_decimal(document.formulario.vlrfin.value) < desformata_decimal(document.formulario.expfinanceiro.value))
    {
    	alert ('O valor gasto não pode ser maior do que o disponível!');
    	document.formulario.expfinanceiro.focus();
		   return;
    }
    if (cod=='I') 
        document.formulario.act2.value = 'insere';
    else if (cod='C') document.formulario.act2.value = 'aprova';
    else if (cod='R') document.formulario.act2.value = 'desaprova';
    else document.formulario.act2.value = 'altera';
  	document.formulario.submit();
  }
  
  	function desformata_decimal( valor )
	{
		if ( valor == null )
		{
			return 0.0;
		}
		valor = valor.replace( /\./g, '' );
		valor = valor.replace( ',', '.' );
		if ( valor[valor.length] == '.' )
		{
			valor += '0';
		}
		if ( valor.length == 0 )
		{
			return 0.0;
		}
		return parseFloat( valor );
	}

</script>


