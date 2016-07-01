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


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
$sql = "select p.* from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where p.prgstatus='A' and p.prsano ='" .$_SESSION['exercicio']."' and p.prgid=".$_SESSION['prgid'];

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if ($nlinhas >= 0) {
if(is_array($res)) foreach($res as $k=>$v) {${$k}=$v;};


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


    include APPRAIZ."includes/cabecalho.inc";
    $db->cria_aba($abacod_tela,$url,'');
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
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
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
        <td><b><?=$prgcod?></b>-<?=campo_textarea('prgdsc','N','N','',50,3,'','dica','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>
        <td><?=campo_textarea('prgdscproblema','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscproblema;prg;text;ppaprograma_proposta;Problema');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo do Programa:</td>
        <td><?=campo_textarea('prgdscobjetivo','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscobjetivo;prg;text;ppaprograma_proposta;Objetivo');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Público-Alvo:</td>
        <td><?=campo_textarea('prgdscpublicoalvo','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscpublicoalvo;prg;text;ppaprograma_proposta;Público Alvo');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=campo_textarea('prgdscjustificativa','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscjustificativa;prg;text;ppaprograma_proposta;Justificativa');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo setorial associado:</td>
        <td><?=campo_textarea('prgdscobjetivoassoc','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscobjetivoassoc;prg;text;ppaprograma_proposta;Objetivo setorial associado');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Desafio associado:</td>
        <td><?=campo_texto('desdsc','N','N','',50,50,'','','','dica',"desafio;descod;des;combo;ppaprograma_proposta;Desafio ;$descod");?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=campo_texto('tprnome','N','N','',50,50,'','','','dica',"tipoprograma;tprcod;tpr;combo;ppaprograma_proposta;Tipo de programa;$tprcod");?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Horizonte temporal:</td>
        <td><?=campo_bool('prgsntemporario','N','N','',50,50,'','','','Temporário',"ppaprograma_proposta;prgsntemporario;prg;bool;ppaprograma_proposta;Horizonte temporal ;$prgsntemporario");?></td>

      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data Início/Término:</td>
        <td><?=campo_texto('prgmesinicio','N','N','',6,2,'','');?> /
	        <?=campo_texto('prganoinicio','N','N','',6,4,'','');?>
	  <br>(Se temporário) <?=campo_texto('prgmestermino','N','N','',6,2,'','');?> /
      <?=campo_texto('prganotermino','N','N','',6,4,'','');?>

	  </td>
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita">Valor Global Estimado <br>do Programa (R$) (se temporário)</td>
        <td>
            <?=campo_texto('prgvlrprograma','N','N','',10,10,'','');?>
	  </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Estratégia de implementação:</td>
        <td><?=campo_textarea('prgdscestrategia','N','N','',50,5,'','dica','ppaprograma_proposta;prgdscestrategia;prg;text;ppaprograma_proposta;Estratégia');?></td>
    </td>
      </tr>


      <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=campo_textarea('prgobs','N','N','',50,5,'','dica','ppaprograma_proposta;prgobs;prg;text;ppaprograma_proposta;Observação');?></td>
      </tr>

<?
   $sql = "select distinct pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo,rpu.usucpf, rpu.prgid, pfl.pflnivel,pfl.pfldsc,usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pflnivel,pfl.pfldsc";

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td align='right' class='SubTituloDireita'><b>".$descricao.":</b></td><td>".$usuario."</td></tr>";
            print $linha;

        }
     }

?>


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
   function edita_campo(tabela_alvo,campo,padrao,tipo_obj,tabela_origem,nome_campo,especial,especial2)
   {
   	 e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostarevisaoprg&acao=A&alvo='+tabela_alvo+'&campo='+campo+'&padrao='+padrao+'&tipo_obj='+tipo_obj+'&origem='+tabela_origem+'&nomcampo='+nome_campo+'&especial='+especial+'&especial2='+especial2;

      window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450");
     // &acao=A&alvo=orgao&campo=orgcod&padrao=org&tipo_obj=combo,acao&tabela_origem=undefined
   }
    function Associa_resp()
    {
          e = "simec.php?modulo=principal/ppafasequalitativa/programa/associa_resp_prg&acao=I";
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
