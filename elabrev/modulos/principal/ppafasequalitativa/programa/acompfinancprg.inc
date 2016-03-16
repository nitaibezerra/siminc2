<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:acompfisicoaca.inc
   finalidade: permitir o acompanhamento físico de ação
   */
$modulo=$_REQUEST['modulo'] ;//
$acao = $_REQUEST['acao'] ;
include "includes/cabecalho.inc";
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];


if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  $db->carrega_registro($RS,$_POST['registro']);
if ($nlinhas >= 0) {
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$_SESSION['prgid']= $prgid;
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Acompanhamento Financeiro do Programa';
monta_titulo($titulo_modulo,'');

// encontrar o orgao
$sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
$res = $db->recuperar($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
// encontrar a unidade
$sql= "select unidsc from unidade where unicod='".$unicod."'";
$res = $db->recuperar($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
<form method="POST"  name="formulario" onsubmit="ProcuraPrograma();">
<input type=hidden name="procura" value=0> 
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',10,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
      </tr>
</form>
      <tr>
        <td align='right' class="subtitulodireita">Orgão Responsável:</td>
        <td><?=$orgdsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Unidade Responsável:</td>
         <td><?=$unidsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$prgdsc?></td>
      </tr>
<?
if ($_REQUEST['refcod']) $refcod=$_REQUEST['refcod'];
   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if (! $mostra ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }

   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

            if ((!$mostra or $mostra=='f') and $usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }
?>
<tr><td align='right' class="SubTituloDireita">Período de Referência :</td>
<td>
<?
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",'Escolhe_referencia()','Todas os períodos de Referências');
?>
</tr>
	 <tr>
        <td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
      </tr>
</table>
<?
$cabecalho = array('Título da Ação','Dotação Atual','Valor Empenhado','Valor Liquidado','Valor Pago');
$sql = "Select '<a href=\"simec.php?modulo=principal/ppafasequalitativa/acao/acompfinancaca&acao=A&acaid=0151\"><font color=#006600><b>'||co_acao||'</b></font></a> - '||no_projeto_atividade, sum(dotacao_disponivel) as disponivel, sum(empenhos_emitidos) as emitidos, sum(empenhos_liquidados) as liquidados, sum(valor_pago) as pago from  siof_simec where co_programa='".$prgcod."' and sfsstatus='A' group by co_acao, no_projeto_atividade order by 1";
$db->monta_lista($sql,$cabecalho,300,20,'S','','');
?>

<script language="JavaScript">
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
