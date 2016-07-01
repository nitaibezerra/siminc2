<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:autorizaesp.inc
   Finalidade: permitir o controle de autorizações especiais
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['act'] == 'inserir')
{
   if (!$_REQUEST['prgid']) $_REQUEST['prgid'] = 'null';
   if (!$_REQUEST['acaid']) $_REQUEST['acaid'] = 'null';
   if (!$_REQUEST['pjeid']) $_REQUEST['pjeid'] = 'null';
   if (!$_REQUEST['saoid']) $_REQUEST['saoid'] = 'null';

   if (!$_REQUEST['refcod'] or $_REQUEST['refcod']==0) {
     $sql="select refcod from referencia";
     $RS = $db->record_set($sql);
     $res =  $db->carrega_registro($RS,0);
     // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
      $_REQUEST['refcod'] = $refcod;
   }

   if ($_REQUEST['acaid'] == 'null')
   {

       if ($_REQUEST['refcod2']<>1)
       {
           $sqlr = "select refcod from referencia where refdata_limite_avaliacao_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refcod=".$_REQUEST['refcod']." and refano_ref='".$_SESSION['exercicio']."'";
        $RSr = $db->record_set($sqlr);
        $nlinhasr = $db->conta_linhas($RSr);
         }
       else
       {
         $sqlr = "select refcod from referencia where refdata_limite_parecer_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."' ";
        $RSr = $db->record_set($sqlr);
        $nlinhasr = $db->conta_linhas($RSr);
       }
        for ($ir=0; $ir<=$nlinhasr;$ir++)
        {

          $resr =  $db->carrega_registro($RSr,$ir);
          $refcod = $resr['refcod'];
          $sql = "insert into autorizacaoespecial (togcod, usucpf, aeldata_inicio, aeldata_fim, prgid,acaid,pjeid,saoid,refcod,aelstatus) values (".
          $_REQUEST['togcod'].",".
          "'".$_SESSION['usucpf']."',".
          "'".$_REQUEST['aeldata_inicio']."',".
           "'".$_REQUEST['aeldata_fim']."',".$_REQUEST['prgid'].",".$_REQUEST['acaid'].",".$_REQUEST['pjeid'].",".$_REQUEST['saoid'].",'".$refcod."','A')";

           $saida = $db->executar($sql);
          }
  }
  else
  {

    $sql="select acacod, unicod from acao where acaid=".$_REQUEST['acaid'];
    $RS = $db->record_set($sql);
    $res =  $db->carrega_registro($RS,0);

     // a linha abaixo transforma em variáveis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
           // caso especial da ação. Inserir para todas as ações que possuam o mesmo código, mesma unidade e não sejam RAP
     $sql="select acaid from acao where prgano='".$_SESSION['exercicio']."' and acacod='".$acacod."' and unicod='".$unicod."'";

     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);

    for ($i=0; $i<=$nlinhas;$i++)
    {
       $res =  $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       $RS2 = $db->record_set($sql);

       if ($_REQUEST['refcod2']<>1)
       {
           $sqlr = "select refcod from referencia where refdata_limite_avaliacao_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refcod=".$_REQUEST['refcod']." and refano_ref='".$_SESSION['exercicio']."'";
        $RSr = $db->record_set($sqlr);
        $nlinhasr = $db->conta_linhas($RSr);
         }
       else
       {
         $sqlr = "select refcod from referencia where refdata_limite_avaliacao_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'";
        $RSr = $db->record_set($sqlr);
        $nlinhasr = $db->conta_linhas($RSr);
       }
        for ($ir=0; $ir<=$nlinhasr;$ir++)
        {

          $resr =  $db->carrega_registro($RSr,$ir);
          $refcod = $resr['refcod'];
       $sql = "insert into autorizacaoespecial (togcod, usucpf, aeldata_inicio, aeldata_fim, prgid,acaid,pjeid,saoid,refcod,aelstatus) values (".
       $_REQUEST['togcod'].",".
       "'".$_SESSION['usucpf']."',".
       "'".$_REQUEST['aeldata_inicio']."',".
       "'".$_REQUEST['aeldata_fim']."',".$_REQUEST['prgid'].",".$acaid.",".$_REQUEST['pjeid'].",".$_REQUEST['saoid'].",'".$refcod."','A')";

       $saida = $db->executar($sql);
       }
    }
  }
	$db->commit();
	$db->sucesso($modulo);

}

include "includes/cabecalho.inc";
?>
<script language="JavaScript" src="includes/calendario.js"></script>
<br>
<?
//$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Administrar autorização especial';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<?
if($_REQUEST['aelid']) {
     $sql= "select ae.*, p.prgcod,p.prgdsc,a.acadsc, a.acacod from autorizacaoespecial ae left join programa p on ae.prgid=p.prgid left join acao a on ae.acaid=a.acaid where ae.aelid=".md5_decrypt($_REQUEST['aelid'],'');
     $saida = $db->recuperar($sql,$res);
     if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;
}
?>
<? } else {
	$togcod = $_POST['togcod'];
	$aeldata_fim =$_POST['aeldata_fim'];
	$aeldata_inicio = $_POST['aeldata_inicio'];
	$refcod = $_POST['refcod'];
	$acacodbusca=$_POST['acacodbusca'];
	$acacodbusca2=$_POST['acacodbusca2'];


 } ?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="aelid" value=<?=$_REQUEST['aelid']?>>
<input type='hidden' name="procura" >
<input type='hidden' name="procuraac" >
<input type='hidden' name="procurapto" >
<input type='hidden' name="act" >
<input type='hidden' name="refcod2" value='0' >


<? if ($_REQUEST['aelid']) {?>
<input type='hidden' name='dtini' value='<?=$aeldata_inicio?>'>
<?}?>
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <td align='right' class="SubTituloDireita">Motivo da autorização especial:</td>
    <td>
	<?
	  $sql = "select togcod as CODIGO,togdsc as DESCRICAO from tipoorigem order by togcod ";
	  $db->monta_combo("togcod",$sql,'S',"Escolha um motivo",'escolhe_fonte','');
	?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Data início:</td>
    <td>
        <?=campo_data('aeldata_inicio', 'S','S','','');?>
	</td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Data fim:</td>
    <td>
       <?=campo_data('aeldata_fim', 'S','S','','');?>
	</td>
  </tr>
  <?
  if ($togcod)
  {
  if ($togcod < 3)
  {// validar ação
      $prgcod=$_REQUEST['prgcod'];
  ?>
     <tr><td align='right' class="SubTituloDireita">Autorização para Validar Ação:</td>
         <td><?=campo_texto('prgcod','N','','',6,6,'','');?>
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='Procura programa' title='Procura programa' onclick="ProcuraPrograma(this.form)"> Entre com o código do programa

         <?
         if ($_REQUEST['procura'] or $prgcod)
         {
           if ($_REQUEST['procura']) $prg = $_REQUEST['procura'] ; else
            $prg = $prgcod;
            $sql= "select prgdsc, prgcod, prgid from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$prg."'  order by prgcod";
            $RS = $db->record_set($sql);
            $nlinhas=$db->conta_linhas($RS);
            if ($nlinhas==0)
            {
              $res =  $db->carrega_registro($RS,0);
            // a linha abaixo transforma em variÃ¡veis todos os campos do array
              if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
              print "<input type='hidden' name='grava' value='1'>";
              print "<input type='hidden' name='prgid' value='".$prgid."'><br><br>";
              print '<b>'.$prgcod.'-'.$prgdsc.'</b>';
            } else
            {
             print '<br><font color="#E41C1A">Programa inexistente</font>';
             print "<input type='hidden' name='grava' value='5'>";
            }
         }
         if ($togcod == 1) // validação de ação
         $sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca < '".date('Y/m/d')."' and refano_ref='".$_SESSION['exercicio']."' order by refcod";

   }
   else if ($togcod == 3 or  $togcod == 4)
   {// avaliação  ou parecer de ação ?>
    <tr>
        <td align='right' class="SubTituloDireita">Autorização para a Ação</td>
        <td><b><input type="text" name="acacodbusca" size="4" value='<?=$acacodbusca?>'><input type="text" name="acacodbusca2" size="5" value='<?=$acacodbusca2?>'><img border="0" src="imagens/busca.gif"  alt='Procura ação' title='Procura ação' onclick="ProcuraAcao(this.form)">Entre com o código (composto) da Ação - (Código da ação - código da Unidade).
      <?
         if ($_REQUEST['procuraac'])
         {
            unset($res2);
            unset ($RS2);
            $sql2= "select prgcod,acadsc, acacod, unicod, loccod, acaid from acao where prgano = '".$_SESSION['exercicio']."' and acacod='".$_REQUEST['acacodbusca']."' and unicod='".$_REQUEST['acacodbusca2']."' order by acacod,unicod";

            $RS2 = $db->record_set($sql2);
            $nlinhas = $db->conta_linhas($RS2);
            if ($nlinhas >= 0) $res2 =  $db->carrega_registro($RS2,0);
            if(is_array($res2)) {
              // a linha abaixo transforma em variáveis todos os campos do array
                foreach($res2 as $k=>$v) ${$k}=$v;
                print "<input type='hidden' name='grava' value='2'>";
                print "<input type='hidden' name='acaid' value='".$acaid."'><br><br>";
                print '<b>'.$acacod.'-'.$unicod.'-'.$acadsc.'</b>';
             } else
             {
             print '<br><font color="#E41C1A">Combinação de ação e unidade inexistente</font>';
             print "<input type='hidden' name='grava' value='6'>";
             }
         }
       }
       else if ($togcod == 5)
       { // sub ação

     }
       else if ($togcod == 6)
       { // projeto especial

     }
       else if ($togcod == 8)
       { // plano de trabalho  ?>
        <tr>
           <td align='right' class="SubTituloDireita">Autorização para a Plano de Trabalho da Ação ou Subação</td>
           <td><b><input type="text" name="acacodpto" size="4" ><input type="text" name="acacodpto2" size="5" ><img border="0" src="imagens/busca.gif"  alt='Procura ação' title='Procura ação' onclick="ProcuraPto(this.form)">Entre com o código (composto) da Ação - (Código da ação - código da Unidade).
      <?
         if ($_REQUEST['procurapto'])
         {
            $sql2= "select prgcod,acadsc, acacod, unicod, loccod, acaid from acao where prgano = '".$_SESSION['exercicio']."' and acacod='".$_REQUEST['acacodpto']."' and unicod='".$_REQUEST['acacodpto2']."' and sacdsc not like '%RAP%' order by acacod,unicod";
            $RS2 = $db->record_set($sql2);
            $res2 =  $db->carrega_registro($RS2,0);
            if(is_array($res2)) {
              // a linha abaixo transforma em variáveis todos os campos do array
                foreach($res2 as $k=>$v) ${$k}=$v;
                print "<input type='hidden' name='grava' value='2'>";
                print "<input type='hidden' name='acaid' value='".$acaid."'><br><br>";
                print '<b>'.$acacod.'-'.$unicod.'-'.$acadsc.'</b>';
             }
         }
     }
     if ($togcod < 8){

         if ($togcod == 1) 
         // validação de ações por programa
         $sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'  order by refcod";
         else if ($togcod == 3)
         $sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_avaliacao_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'  order by refmes_ref";
         
         else if ($togcod > 4)
         $sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_avaliacao_aca < '".date('Y/m/d')."' or refdata_limite_parecer_aca < '".date('Y/m/d')."' and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'  order by refcod";

         ?>
    </td></tr>
      
         <tr>
        <td align='right' class="SubTituloDireita">Período de Referência:</td>
	<td>
	<?

	  $db->monta_combo("refcod",$sql2,'S',"Todos os períodos com datas vencidas",'','');
	?>
	</td>
      </tr>
  <?}else {?><input type='hidden' name="refcod" value='0' ><?}}?>

<? if   ($_REQUEST["aelid"]) { ?>

<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_alt('A')" class="botao">
   <input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
 </tr>
<? } else { ?>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"></td>

 </tr>
<? } ?>
    </table>
    <br><br>

</form>
<?
$cabecalho = array('Autorização especial','Referência','Autorizado por','Inicio','Fim');
$sql= "select case when ae.togcod =1 then '<b>Validação de Ação</b> - '||prg.prgcod || '-' || prg.prgdsc when ae.togcod =2 then '<b>Validação de Ação</b> - '||prg.prgcod || '-' || prg.prgdsc when ae.togcod =3 then '<b>Avaliação de Ação</b> - ' || aca.acacod ||'.'|| aca.unicod ||'-' || aca.acadsc when ae.togcod =4 then '<b>Avaliação de Ação</b> - '||aca.acadsc when ae.togcod=5 then 'Subação - '||sao.saodsc when ae.togcod=6 then '<b>Projeto Especial</b> - '||pje.pjedsc when ae.togcod =8 then 'Plano de Trabalho - ' || aca.acacod ||'-'|| aca.unicod ||'-' || aca.acadsc end as descricao , ref.refdsc,usu.usunome, ae.aeldata_inicio, ae.aeldata_fim from autorizacaoespecial ae left join programa prg on prg.prgid=ae.prgid left join acao aca on aca.acaid = ae.acaid left join projetoespecial pje on pje.pjeid=ae.pjeid left join subacao sao on sao.saoid=ae.saoid  left join usuario usu on usu.usucpf= ae.usucpf left join referencia ref on ref.refcod=ae.refcod where ae.aelstatus = 'A' order by ae.refcod";


$db->monta_lista($sql,$cabecalho,50,20,'','','');


?>



<script>
  function alterar_aut(cod)
  {
    document.formulario.aelid.value = cod;
    document.formulario.submit();
  }
  function escolhe_fonte()
  {
       document.formulario.submit();
  }
    function ProcuraPrograma(form)
    {
	   document.formulario.procura.value = document.formulario.prgcod.value;
	   document.formulario.submit();
  }
    function ProcuraAcao(form) {
      if (!validaBranco(document.formulario.acacodbusca, 'Código da ação')) return;
      if (!validaBranco(document.formulario.acacodbusca2, 'Código da unidade')) return;
      document.formulario.procuraac.value = document.formulario.acacodbusca.value;
      document.formulario.submit();
    }
    
    function ProcuraPto(form) {
	document.formulario.procurapto.value = document.formulario.acacodpto.value;
	document.formulario.submit();
    }
    
   function validar_cadastro(cod)
   {
	if (!validaBranco(document.formulario.togcod, 'Motivo da autorização')) return;
    if (!validaData(document.formulario.aeldata_inicio))
    {
    	alert("Data de Início Inválida.");
		document.formulario.aeldata_inicio.focus();
		return;
    }
    if (!validaData(document.formulario.aeldata_fim))
    {
    	alert("Data de Fim Inválida.");
		document.formulario.aeldata_fim.focus();
		return;
    }
    if (!validaDataMaior(document.formulario.aeldata_inicio , document.formulario.aeldata_fim))
		{
				alert("Data Fim  não pode ser Anterior que Data Inicio.");
				document.formulario.aeldata_fim.focus();
				return;
		}
    if (document.formulario.grava.value == 5)
    {
      alert ('Escolha uma programa válido.');
      document.formulario.prgcod.focus();
      return;
    }
    if (document.formulario.grava.value == 6)
    {
      alert ('Escolha uma ação válida.');
      document.formulario.acacodbusca.focus();
      return;
    }
    if (document.formulario.grava.value == 7)
    {
      alert ('Escolha um Projeto Especial válido.');
      document.formulario.pjecod.focus();
      return;
    }
	if (document.formulario.refcod.value == '')
    {
        if( window.confirm('Você escolheu todos os períodos com data vencidas! \nConfirma sua escolha?'))
        {
            document.formulario.refcod2.value = 1;
    } else return;
    }
    

	document.formulario.act.value='inserir';
    document.formulario.submit();

}    
</script>
