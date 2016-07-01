<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:cadprograma.inc
   Finalidade: permitir a alteração dos atributos do programa
   */

$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
$prgid = (integer) $_REQUEST['prgid'];

if ($_REQUEST['prgid'] or $_REQUEST['verprgid'])
{
	if ($_REQUEST['verprgid']) $_REQUEST['prgid']=$_REQUEST['verprgid'];
     $sql = "select p.*,t.tprnome, d.desdsc from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p inner join tipoprograma t on t.tprcod=p.tprcod left join desafio d on d.descod=p.descod where p.prgid=".$_REQUEST['prgid'];
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    $_SESSION['prgid']=$_REQUEST['prgid'];
}
//

include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Atributos do Programa';
monta_titulo($titulo_modulo,'');
// variáveis gerais

?>

<script language="Javascript" src="/includes/livesearch.js"></script>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="clone" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="prog_n">
<input type=hidden name="verprgid" value=0>
<input type=hidden name="prgid" value=<?=$_REQUEST['prgid']?>>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
   <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?
   $orgcod =$_SESSION["ittorgao"];
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao where organo='".$_SESSION['exercicio']."' order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'N',"Todos os Órgãos",'atualizaComboUnidade','','MEC',400);
	?></td>
      </tr>
       <tr>
	      <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=campo_texto('tprnome','N','N','',50,50,'','','','dica',"tipoprograma;tprcod;tpr;combo;ppaprograma_proposta;Tipo do Programa ;$tprcod");?>
			</td>           
       </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Título do programa:</td>
        <td><?=campo_textarea('prgdsc','N','N','',80,3,'','','','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo:</td>
        <td><?=campo_textarea('prgdscobjetivo','N','N','',80,3,'','dica','ppaprograma_proposta;prgdscobjetivo;prg;text;ppaprograma_proposta;Objetivo');?></td>         
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Público alvo:</td>  
        <td><?=campo_textarea('prgdscpublicoalvo','N','N','',80,3,'','dica','ppaprograma_proposta;prgdscpublicoalvo;prg;text;ppaprograma_proposta;Público Alvo');?></td>          
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>  
        <td><?=campo_textarea('prgdscjustificativa','N','N','',80,3,'','dica','ppaprograma_proposta;prgdscjustificativa;prg;text;ppaprograma_proposta;Justificativa');?></td>          
      </tr> 
      <tr>
        <td align='right' class="SubTituloDireita">Estratégia:</td>  
        <td><?=campo_textarea('prgdscestrategia','N','N','',80,3,'','dica','ppaprograma_proposta;prgdscestrategia;prg;text;ppaprograma_proposta;Estratégia');?></td>          
      </tr> 
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>  
        <td><?=campo_textarea('prgdscproblema','N','N','',80,3,'','dica','ppaprograma_proposta;prgdscproblema;prg;text;ppaprograma_proposta;Problema');?></td>
           
      </tr> 
       <tr>
        <td align='right' class="SubTituloDireita">Desafio:</td>
        <td><?=campo_texto('desdsc','N','N','',50,50,'','','','dica',"desafio;descod;des;combo;ppaprograma_proposta;Desafio ;$descod")?>
	</td> 
      </tr>            
      <tr>
        <td align='right' class="SubTituloDireita">Valor estimado:</td>  
        <td><?=campo_texto('prgvlrprograma','N','N','Entre com o valor em Reais em inteiros',50,15,'##################','');?></td>          
      </tr>      
<tr>
        <td align='right' class="SubTituloDireita">Moldura temporal:</td>
        <td><input type="radio" value="f" name="prgsntemporario" <?if ($prgsntemporario=='f') print '   checked'?>>Contínuo     <input type="radio" value="t" name="prgsntemporario" <?if ($prgsntemporario=='t') print '   checked'?>>Temporário</td>
      </tr>              
    
 <? if ($prgsntemporario=='t'){?>
	<tr>
	   <td align='right' class="SubTituloDireita">Período do Programa<br>(Para programas temporários):</td>
       <td>
          <table border=1>
             <tr>
                <td width='50%'>Início</td>
                <td width='50%'>Término</td>
             </tr>
             <tr>
                <td width='50%'><?=campo_texto('prgmesinicio','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('prganoinicio','N',$habil,'',6,4,'####','N');?></td>
                <td width='50%'><?=campo_texto('prgmestermino','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('prganotermino','N',$habil,'',6,4,'####','N');?></td>
             </tr>
          </table>
       </td>
    </tr> 
    <?}?>                 

     <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=campo_textarea('prgobs','N','N','',80,3,'','dica','ppaprograma_proposta;prgobs;prg;text;ppaprograma_proposta;Observações');?></td>
      </tr>     
  <?
  if ($db->verifica_momento())
  {

   if (! $_REQUEST['prgid']){?>
      
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_janela('I')"></tr>
      <? } else if ($db->ehcriador($_REQUEST['prgid'],'P')){?>
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Alterar' onclick="gravar_janela('A')"></tr>      
      <?}
  }?>
    </table>
</form>  

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
  function popfun(prgid)
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostafusao_programa&acao=A&prgid='+prgid;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }
  function geraclone()
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/geraclone&acao=P';
		window.open(e, "GerarClone","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=250");
  }  
  function tipomoldura(cod)
  {
	document.formulario.submit();
  }  
  
  
   function gravar_janela(cod)
   {	
   	  //prepara_formulario();
   	  // testar todos os campos antes de incluir

   	  if (!validaBranco(document.formulario.tprcod, 'Tipo de programa')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdsc, 'Título')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdscobjetivo, 'Objetivo')) return;
   	  if (!validaBranco(document.formulario.prgdscpublicoalvo, 'Público alvo')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdscjustificativa, 'Justificativa')) return;
   	  if (!validaBranco(document.formulario.prgdscestrategia, 'Estratégia')) return;
   	  if (!validaBranco(document.formulario.prgdscproblema, 'Problema')) return;
   	 	  
   	  if ( !validar_radio( document.formulario.prgsntemporario, 'Moldura temporal' ) ) {
			alert ('Moldura temporal!');
			document.formulario.prgsntemporario.focus();
			return;
		} 
	  
/*
   	  if (document.formulario.prgsntemporario[1].checked)     	     	  
   	  { // é temporário
   	  	if (document.formulario.prgmesinicio.value <0 ||  document.formulario.prgmesinicio.value >12)
   	  	{
   	  		alert ('Mês de início inválido!');
			document.formulario.prgmesinicio.focus();
			return;
   	  	}
   	  	if (document.formulario.prgmestermino.value <0 ||  document.formulario.prgmestermino.value >12)
   	  	{
   	  		alert ('Mês de término inválido!');
			document.formulario.prgmestermino.focus();
			return;
   	  	}   	  	

 	  	if (!validaBranco(document.formulario.prgmesinicio, 'Mês de início do programa')) return; 
   	  	if (!validaBranco(document.formulario.prganoinicio, 'Ano de início do programa')) return; 
   	  	if (!validaBranco(document.formulario.prgmestermino, 'Mês de término do programa')) return; 
   	  	if (!validaBranco(document.formulario.prganotermino, 'Ano de término do programa')) return;    
   	  	if (document.formulario.prganotermino.value < document.formulario.prganoinicio.value)	  	{
   	  		alert ('Ano de término do programa não pode ser anterior ao de início');
   	  		return;
   	  	}
   	  	else if (document.formulario.prganotermino.value == document.formulario.prganoinicio.value && document.formulario.prgmestermino.value < document.formulario.prgmesinicio.value)
   	  	{
   	  		alert ('Mês de término do programa não pode ser anterior ao de início');
   	  		document.formulario.acamestermino.focus();
   	  		return;   	  		
   	  	}
   	  	  	
   	  }
*/ 	  
   	  if (cod=='I') document.formulario.prog_n.value='inclui';
   	  else document.formulario.prog_n.value='altera';
   	   	   document.formulario.submit();
   }
  
    
</script>

