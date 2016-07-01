<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadreferencia.inc
   Finalidade: permitir o controle de cadastro das referencias de acompanhamento
   */
$modulo=$_REQUEST['modulo'] ;//
/*
elabrev.referencia
(
  refcod int4 NOT NULL DEFAULT nextval('referencia_refcod_seq'::regclass),
  refdata_inicio date,
  refdata_limite_momento1 date,
  refdata_inc date DEFAULT ('now'::text)::timestamp(6) without time zone,
  refdata_ult_alt date,
  refdsc varchar(45) NOT NULL,
  refmes_ref char(2) NOT NULL,
  refano_ref char(4) NOT NULL,
  refsnmonitoramento bool,
  refstatus char(1) DEFAULT 'A'::bpchar,
  refdata_limite_momento2 date,
  refdata_limite_momento3 date,
  refdata_limite_elabrev date,
*/


if ($_REQUEST['act'] == 'I')
{
   // fazer inserção de referencia na base de dados.
   $sql = "insert into elabrev.referencia (
      refdata_inicio,
      refdata_limite_momento1,
      refdata_limite_momento2,
      refdsc,
      refano_ref,
      refsnmonitoramento,
      refdata_ult_alt)
      values
      (
      '".formata_data_sql($_REQUEST['refdata_inicio'])."','".formata_data_sql($_REQUEST['refdata_limite_momento1'])."','".
      formata_data_sql($_REQUEST['refdata_limite_momento2'])."','Controle ano ".$_REQUEST['refano_ref']."','".
      $_REQUEST['refano_ref']."','".$_REQUEST['refsnmonitoramento']."','".date('Y-m-d')."')";
 }

if ($_REQUEST['act']=='A')
{
   // fazer alteração de referencia na base de dados.

   if (! $_REQUEST['coord']) $_REQUEST['refsnmonitoramento']='f';
   if (! $_REQUEST['refdata_limite_elabrev']) $_REQUEST['refdata_limite_elabrev']='null'; else $_REQUEST['refdata_limite_elabrev'] = "'".$_REQUEST['refdata_limite_elabrev']."'";  
   $sql = "update elabrev.referencia set refdata_limite_elabrev=".
   $_REQUEST['refdata_limite_elabrev'].
   ",  refdata_inicio='".$_REQUEST['refdata_inicio'].
   "',   refdata_ult_alt='".
   date('d/m/Y').
   "',  refsnmonitoramento='".$_REQUEST['refsnmonitoramento'].
   "'  where refcod=".$_REQUEST['refcod'];
   
      // fazer inserção de referencia na base de dados.
   $sql = "update elabrev.referencia set refdata_inicio='".$_REQUEST['refdata_inicio']."',
      refdata_limite_momento1='".$_REQUEST['refdata_limite_momento1']."',
      refdata_limite_momento2='".$_REQUEST['refdata_limite_momento2']."',      
      refsnmonitoramento='".$_REQUEST['refsnmonitoramento']."',
      refdata_ult_alt=now()";
        
}
if ($_POST['exclui'] > 0) 
{
    $sql = "update elabrev.referencia set refstatus='I' where refcod=".$_POST['exclui'];
    $saida = $db->executar($sql);
    unset($_POST['exclui']);   
    $db->commit();
    $db->sucesso($modulo);
}


	if ($_REQUEST['act']=='A' or $_REQUEST['act']=='I')
{

   $saida = $db->executar($sql);
   $db->commit();
   if ($_REQUEST['refsnmonitoramento']=='t')
   {
         
     if ($_REQUEST['refmes_ref']=='01' or $_REQUEST['refmes_ref']=='02' or $_REQUEST['refmes_ref'] =='03')
          $trimestre = '1º';
     if ($_REQUEST['refmes_ref']=='04' or $_REQUEST['refmes_ref']=='05' or $_REQUEST['refmes_ref'] =='06')
          $trimestre = '2º';
     if ($_REQUEST['refmes_ref']=='07' or $_REQUEST['refmes_ref']=='08' or $_REQUEST['refmes_ref'] =='09')
          $trimestre = '3º';
     if ($_REQUEST['refmes_ref']=='10' or $_REQUEST['refmes_ref']=='11' or $_REQUEST['refmes_ref'] =='12')
          $trimestre = '4º';

        // envia-email para os gerentes, gerentes-executivos, coordenadores de ação, super usuário, equipes de apoio e UMA.
        $sql="select distinct u.usunome,u.usuemail from usuario u inner join perfilusuario pfu on pfu.pflcod in (1,2,3,6,8,18) and pfu.usucpf=u.usucpf";
         $RSu = $db->record_set($sql);
         $nlinhas = $db->conta_linhas($RSu);
         for ($i=0;$i<=$nlinhas;$i++)
         {
             $resu =  $db->carrega_registro($RSu,$i);
             if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
             $assunto = 'Liberação de Período de Referência';
             if ($_REQUEST['act']=='inserir'){
             		$mensagem = 'Prezado usuário,';
             		$mensagem = $mensagem.' <br> Foi liberado no SIMEC o período de '.mes_extenso($_REQUEST['refmes_ref']).'/'.$_REQUEST['refano_ref'].' para monitoramento. Lembramos que as informações relativas à execução física das ações dos programas que compõem o Plano Plurianual devem ser registradas, no <b>SIMEC</b>, obedecendo o seguinte cronograma:';
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento da <b>AVALIAÇÃO</b> das ações, pelo <b>COORDENADOR DE AÇÃO</b>, referente ao mês de '.mes_extenso($_REQUEST['refmes_ref']).'/'.$_REQUEST['refano_ref'].': até o dia '.$_REQUEST['refdata_limite_avaliacao_aca'];
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento do <b>ACOMPANHAMENTO FÍSICO</b> das ações, pelo <b>COORDENADOR DE AÇÃO</b>, referente ao mês de '.mes_extenso($_REQUEST['refmes_ref']).'/'.$_REQUEST['refano_ref'].': até o dia  '.$_REQUEST['refdata_limite_acomp_fisico'];
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento do <b>PARECER</b> dos programas, pelo <b>GERENTE DE PROGRAMA</b>, referente ao 1º trimestre de '.$_REQUEST['refano_ref'].': até o dia '.$_REQUEST['refdata_limite_parecer_prg'];
           }  else{
                    $mensagem = 'Prezado usuário,';
                    $mensagem = $mensagem.' <br> Em virtude das solicitações dos coordenadores de ação e gerentes de programa e, tendo em vista o período em que muitos servidores encontram-se em férias,';
                    $mensagem = $mensagem.'  a Unidade de Monitoramento e Avaliação do MEC decidiu prorrogar as datas para o preenchimento das informações referentes à avaliação e ao acompanhamento';
                    $mensagem = $mensagem.'  físico das ações dos programas que compõem o Plano Plurianual, obedecendo ao seguinte cronograma:';
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento da <b>AVALIAÇÃO</b> das ações, pelo <b>COORDENADOR DE AÇÃO</b>, referente ao mês de '.mes_extenso($_REQUEST['refmes_ref']).'/'.$_REQUEST['refano_ref'].': até o dia '.$_REQUEST['refdata_limite_avaliacao_aca'];
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento do <b>ACOMPANHAMENTO FÍSICO</b>  das ações, pelo <b>COORDENADOR DE AÇÃO</b>, referente ao mês de '.mes_extenso($_REQUEST['refmes_ref']).'/'.$_REQUEST['refano_ref'].': até o dia  '.$_REQUEST['refdata_limite_acomp_fisico'];
	      		$mensagem = $mensagem.' <br> - Data limite para o preenchimento do <b>PARECER</b> dos programas, pelo <b>GERENTE DE PROGRAMA</b>, referente ao '.$trimestre.' trimestre de '.$_REQUEST['refano_ref'].': até o dia '.$_REQUEST['refdata_limite_parecer_prg'];
                    
       }
    //email(str_to_upper($usunome), $usuemail, $assunto, $mensagem);
             
     }
    // email(str_to_upper($gestor), $emailproprio, $assunto, $mensagem);
    }

    $db->sucesso($modulo);
} 
include APPRAIZ."includes/cabecalho.inc";
?>
<title>Cadastro de Períodos de Referência de Acompanhamento do Simec</title>
<script language="JavaScript" src="../includes/calendario.js"></script>
<div align="center">
<?
if($_REQUEST['refcod']) {   
        $sql= "select * from elabrev.referencia where refcod=".$_REQUEST['refcod'];
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;
    }
?>
<? } else { 
	$refdata_inicio = $_REQUEST['refdata_inicio'];
	$refdata_limite_elabrev =$_REQUEST['refdata_limite_elabrev'];
	$refdata_inc=$_REQUEST['refdata_inc'];
	$refdata_ult_alt = $_REQUEST['refdata_ult_alt'];
	$refdsc = $_REQUEST['refdsc'];
	$refmes_ref = $_REQUEST['refmes_ref'];
	$refano_ref = $_REQUEST['refano_ref'];
	$refsnmonitoramento = $_REQUEST['refsnmonitoramento'];
	$refstatus = $_REQUEST['refstatus'];
	$refcod = $_REQUEST['refcod'];
 }
print '<br>';
$titulo_modulo='Cadastro de Períodos de Referência';
monta_titulo($titulo_modulo,'');
 ?>
<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="refcod" value=<?=$_REQUEST['refcod']?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act' value=0>
<? if ($_REQUEST['refcod']) {?>
<input type='hidden' name='dtini' value='<?=formata_data($refdata_inicio)?>'>
<?}?>  
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Ano de referência:</td>
        <td>
		<?$sql='select ano as codigo, ano as descricao from anos';
        $db->monta_combo("refano_ref",$sql,'S',"Selecione o ano",'','');?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data de início:</td>
        <td><?=campo_data('refdata_inicio', '','S','','S');?>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data limite de proposição Momento UO:</td>
        <td>
           <?=campo_data('refdata_limite_momento1', '','S','','S');?>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data limite de proposição Momento SPO:</td>
        <td>
           <?=campo_data('refdata_limite_momento2', '','S','','S');?>
	    </td>
      </tr>      
      <tr>
        <td colspan=2  align='center' bgcolor="#cdcdcd"><hr></td>
      </tr>
      <?
        // verifica se é coordenador do simec ou super-usuário
  
  if ($db->testa_superuser() or $db->testa_uma2()) { ?>
      <input type='hidden' name='coord' value='S'>
      <tr>
        <td align='right' class="SubTituloDireita">Libera para Monitoramento?</td>
	<? if ($refsnmonitoramento == 'f') {?>
        <td><input type="radio" name="refsnmonitoramento" value='t'>Sim&nbsp;&nbsp;<input type="radio" name="refsnmonitoramento" value='f' checked>Não</td>
       <?;} else {?>
        <td ><input type="radio" name="refsnmonitoramento" value='t' checked>Sim&nbsp;&nbsp;<input type="radio" name="refsnmonitoramento" value='f' >Não<?=obrigatorio();?></td>
      <?} ?>
      </tr>
      <? }?>
      
<? if   ($_REQUEST["refcod"]) { ?>
<tr bgcolor="#C0C0C0">
   <td></td>
   <td><input type="button" class="botao" name="btalterar" value="Atualizar" onclick="validar_cadastro('A');"><input type="button" class="botao" name="btcancelar" value="Voltar" onclick="history.back();"></td>
 </tr>
<? } else { ?>
<tr bgcolor="#C0C0C0">
   <td></td>
   <td><input type="button" class="botao" name="btinserir" value="Incluir" onclick="validar_cadastro('I');"></td>
 </tr>
<? } ?>
    </table>
    </form>
    <br><br>
<?
$cabecalho = array('Ações','Descrição');
$sql= "select  '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Referência \" onclick=\"altera_ref('||refcod||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Referência\" onclick=\"excluir_ref('||refcod||')\">' as acao,  refdsc as descricao from elabrev.referencia where refstatus = 'A' and refano_ref='".$_SESSION['exercicio']."'  order by refano_ref, refmes_ref";


$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>

<script>
	function validar_cadastro(cod)
    {
	    if (!validaBranco(document.formulario.refano_ref, 'Ano de referência')) return ;
		if (!validaData(document.formulario.refdata_inicio))
		{
			alert("Data Inicio Inválida.");
			document.formulario.refdata_inicio.focus();
			return;
		}
		if (!validaData(document.formulario.refdata_limite_momento1))
		{
			alert("Data limite proposição momento 1 Inválida.");
			document.formulario.refdata_limite_momento1.focus();
			return;
		}		
		if (!validaDataMaior(document.formulario.refdata_inicio, document.formulario.refdata_limite_momento1))
		{
			alert("Data Limite de Proposição Momento 1 não pode ser Anterior que Data de Inicio.");
			document.formulario.refdata_limite_momento1.focus();
			return;
		}

		if (!validaData(document.formulario.refdata_limite_momento2))
		{
			alert("Data limite proposição momento 2 Inválida.");
			document.formulario.refdata_limite_momento2.focus();
			return;
		}

		if (!validaDataMaior(document.formulario.refdata_limite_momento1, document.formulario.refdata_limite_momento2))
		{
			alert("Data Limite de Proposição Momento 2 \nnão pode ser Anterior que Data Limite de Proposição Momento 1.");
			document.formulario.refdata_limite_momento2.focus();
			return;
		}		
				
	//if (cod == 'I') document.formulario.act.value = 'inserir'; //else 
	    document.formulario.act.value = cod;
   	  	document.formulario.submit();
	}
  function altera_ref(cod) {
    document.formulario.refcod.value = cod;
    document.formulario.submit();
  }
  function excluir_ref(cod) {
    if( window.confirm( "Confirma a exclusão da Referência?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
  }
</script>
