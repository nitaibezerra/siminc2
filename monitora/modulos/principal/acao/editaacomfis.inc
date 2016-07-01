<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:editaacomfis.inc
   Finalidade: permitir a edição de acompanhamento físico
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];
if (! $_SESSION['acaid'])
{
	   ?>
	      <script>
	         alert ('É necessário escolher uma Ação na opção Ações - Consultar');
             window.close();
	      </script>
	   <?
	     exit();
}
?>
<?
  $autorizado = false;
  // verifica se é coordenador de ação
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=1 and acaid='.$_SESSION['acaid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) $autorizado = true;
  // verifica se é digitador
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=8 and acaid='.$_SESSION['acaid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) $autorizado = true;
  // testo se é super usuário
$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))
     $autorizado = true;

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
if ($_REQUEST['act']=='grava')
{
  if (! $_REQUEST['expobs']) $_REQUEST['expobs'] = '';
  if ($_REQUEST['alteracao']==1)
  {
     // é uma alteração
     if ($_REQUEST['tpscod'])
        $sql = "update execucaopto set exprealizado=".$_REQUEST['exprealizado'].",usucpf='".$_SESSION['usucpf']."',expobs='".$_REQUEST['expobs']."',tpscod=".$_REQUEST['tpscod']." where expid= ".$_REQUEST['expid'];
     else
        $sql = "update execucaopto set exprealizado=".$_REQUEST['exprealizado'].",usucpf='".$_SESSION['usucpf']."',expobs='".$_REQUEST['expobs']."' where expid= ".$_REQUEST['expid'];
     
  }
  else
  {
    // é uma inclusão
     if ($_REQUEST['tpscod'])
        $sql = "insert into execucaopto (ptoid,refcod,exprealizado,usucpf,expobs,tpscod) values (".$_REQUEST['ptoid'].",".$_REQUEST['refcod'].",".$_REQUEST['exprealizado'].",'".$_SESSION['usucpf']."','".$_REQUEST['expobs']."',".$_REQUEST['tpscod'].")";
     else
        $sql = "insert into execucaopto (ptoid,refcod,exprealizado,usucpf,expobs) values (".$_REQUEST['ptoid'].",".$_REQUEST['refcod'].",".$_REQUEST['exprealizado'].",'".$_SESSION['usucpf']."','".$_REQUEST['expobs']."')";
  }

    $res = $db->executar($sql);
    $db->commit();
    	   ?>
	      <script>
	         alert ('Transação realizada com sucesso!');
             window.close();
                 opener.location.reload();
	      </script>
	   <?
	     exit();
}

unset($res);
$sql= "select sum(exprealizado) as realizado from execucaopto where ptoid=".$_REQUEST['ptoid'];
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);
$realizado = $res[0];
if (! $realizado) $realizado=0;
unset($res);

$sql= "select a.prgcod,a.acaid, a.acacod, a.acadsc,a.loccod , a.unicod,p.ptodsc,p.ptoprevistoexercicio, p.ptodata_ini,p.ptodata_fim,u.unmdsc from acao a, planotrabalho p, unidademedida u where a.acaid=p.acaid and a.acastatus='A' and u.unmcod=p.unmcod and p.ptoid =".$_REQUEST['ptoid'];
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
unset($res);
$sql= "select refdsc from referencia  where refcod=".$_REQUEST['refcod'];
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
unset($res);
$sql= "select * from execucaopto  where refcod=".$_REQUEST['refcod']." and ptoid=".$_REQUEST['ptoid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
?>

<title>Editar Acompanhamento Físico de Ação</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="includes/funcoes.js"></script>
<div align="center">
<table  width='95%' border="0" cellspacing="0" cellpadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Editar Acompanhamento Físico de Ação</label></td></tr>
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="refcod" value="<?=$_REQUEST['refcod']?>">
<input type=hidden name="ptoid" value="<?=$_REQUEST['ptoid']?>">
<?
if ($nlinhas >=0)
{
    $res =  $db->carrega_registro($RS,0);
    print '<input type=hidden name="alteracao" value=1>';
}
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<input type=hidden name="expid" value="<?=$expid?>">
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
      <tr>
        <td align='right' class="subtitulodireita">Ação:</td>
        <td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?></td>
      </tr>
	<tr>
     <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Plano de Trabalho:</td><td><?=$ptodsc?></td>
      </tr>
	<tr>
     <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Datas de Início e Término:</td><td><?=formata_data($ptodata_ini).' a '.formata_data($ptodata_fim)?></td>
      </tr>
	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Referência:</td><td><?=$refdsc?></td>
      </tr>
	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Previsto no Exercício:</td><td><?=$ptoprevistoexercicio?></td>
      </tr>
	<tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Realizado até o momento:</td><td><?=$realizado?></td>
      </tr>
	  	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Unidade de Medida:</td><td><?=$unmdsc?></td>
      </tr>
	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Executado:</td><td><?=campo_texto('exprealizado','S',$habil,'',8,8,'########','');?></td>
      </tr>
	 <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Situação:</td><td>
<?
	  $sql = "select tpscod as CODIGO,tpsdsc as DESCRICAO from tiposituacao where tpsstatus='A'  order by tpsdsc ";
	  $db->monta_combo("tpscod",$sql,'S',"Selecione a situação",'','');
?>
       </td>
     </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Observações:</td><td><?=campo_textarea("expobs",'N',"S",'',85,7,'')?></td>
    </tr>
    <tr bgcolor="#C0C0C0">
       <td colspan=2><input type="button" class="botao" name="btgravar" value="Gravar" onclick="validar_cadastro()"><input type="reset" class="botao" value="Limpar" ></td>
    </tr>
</table>
<table>
  <tr><td><input type='button' class="botao" value='Fechar Janela' onclick="fechar_janela()">
</table>
</form>


<script>
  function fechar_janela()
  {
    window.close();
    opener.location.reload();
  }
  function validar_cadastro()
  {
    if (!validaBranco(document.formulario.exprealizado, 'Realizado')) return;
    document.formulario.act.value = 'grava';
  	document.formulario.submit();
  }

</script>


