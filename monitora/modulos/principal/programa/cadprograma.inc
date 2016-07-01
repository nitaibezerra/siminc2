<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadprograma.inc
   Finalidade: permitir o controle do cadastro de programas
   */

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";

?>
<title>Cadastro de Programas do Simec</title>
<br>
<?
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  $db->carrega_registro($RS,$_POST['registro']);
if ($nlinhas >= 0) {
if(is_array($res)) foreach($res as $k=>$v) {${$k}=$v;};
$_SESSION['prgid']= $prgid;

	// encontrar o desafio (macro-objetivo)
   $sql= "select mobdsc from macroobjetivo where mobcod='".$mobcod."'";
   $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$unicod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o tipo de programa
    $sql= "select tprnome from tipoprograma where tprcod='".$tprcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>
<?$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Atributos de Programa';
monta_titulo($titulo_modulo,'');

?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario" onsubmit="ProcuraPrograma();">
<input type=hidden name="procura" value=0>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',8,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="../imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
      </tr>
</form>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão responsável:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <? if ($unidsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade responsável:</td>
         <td><?=campo_texto('unidsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
        <td><?=campo_texto('prgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <? if ($prgdscproblema) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>
        <td><?=campo_textarea('prgdscproblema','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscobjetivo) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo do Programa:</td>
        <td><?=campo_textarea('prgdscobjetivo','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscpublicoalvo) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Público-Alvo:</td>
        <td><?=campo_textarea('prgdscpublicoalvo','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscjustificativa) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=campo_textarea('prgdscjustificativa','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgdscobjetivoassoc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo setorial associado:</td>
        <td><?=campo_textarea('prgdscobjetivoassoc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($mobdsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Desafio associado:</td>
        <td><?=campo_textarea('mobdsc','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($prgsnestruturante) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Programa estruturante?</td>
        <td>
	<?
	   if ($prgsnestruturante =='t') {
	      print '<input type="radio" value="1" name="prgsnestruturante" checked>Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="0" checked>Não';
	   }
	   else
	   {
	     print '<input type="radio" value="0" name="prgsnestruturante"> Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="1"  checked>Não';
	   }
      ?>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=campo_texto('tprnome','N','N','',30,30,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Horizonte temporal:</td>
        <td>
	<?
	   if ($prgsntemporario=='t')
	   {
	      print '<input type="radio" value="0" name="prgsntemporario"> Contínuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="1" checked>Temporário';
	   }
	   else
	   {
	      print '<input type="radio" value="1" name="prgsntemporario" checked> Contínuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="0" >Temporário';
	   }
	?>
      </tr>
	<?
	   if ($prgsntemporario=='t')
	   {?>
      <tr>
        <td align='right' class="SubTituloDireita">Data Início/Término:</td>
        <td><?=campo_texto('prgmesinicio','N','N','',6,2,'','');?> /
	        <?=campo_texto('prganoinicio','N','N','',6,4,'','');?>
	<? if ($prgsntemporario) {
	?>
	  <br>(Se temporário) <?=campo_texto('prgmestermino','N','N','',6,2,'','');?> /
      <?=campo_texto('prganotermino','N','N','',6,4,'','');?>
	  <?}?>
	  </td>
      </tr>
      <?}?>
	<? if ($prgsntemporario=='t') {	?>
       <tr>
        <td align='right' class="SubTituloDireita">Valor Global Estimado <br>do Programa (R$) (se temporário)</td>
        <td>
            <?=campo_texto('prgvlrprograma','N','N','',10,10,'','');?>
	  </td>
      </tr>
	  <?}?>
	<?	   if ($prgdscestrategia) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Estratégia de implementação:</td>
        <td><?=campo_textarea('prgdscestrategia','N','N','',100,5,'');?></td>
    </td>
      </tr>
	  <?}?>
	<?	  if ($prgobs) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=campo_textarea('prgobs','N','N','',100,5,'');?></td>
      </tr>
	  <?}?>
	  <?$db -> mostra_resp($_SESSION['prgid'], 'prgid');?>

<? if ($db->testa_uma()) { ?>
  <tr bgcolor="#cccccc">
      <td></td>
   <td><input type="button" class="botao" name="btassociar" value="Associar Responsáveis" onclick="Associa_resp()"></td>
      </tr>
  <?}?>

    </table> 

  </center>
</div>

<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
	function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Associa_resp()
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/associa_resp_prg&acao=I";
          WindowObjectReference = window.open(e, "Associação_de_Responsáveis",
"menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
    }
function ProcuraPrograma() {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
</script>

<?} else {?>
<script language="JavaScript">
alert('Programa "<?=$_REQUEST['procura'];?>" não encontrado!\n\nTente Novamente.');
history.back();
</script>
<?}?>
</body>
</html>
