<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:propaltprograma.inc
   Finalidade: permitir a proposta de exclusao do ppa
   */
 
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
$prgid = (integer) $_REQUEST['prgid'];
$lista=$_REQUEST['lista'];

if ($_REQUEST['prgid'] and $_REQUEST['pepjustificativa'] and $_REQUEST['prog_n']=='E')
{
	// há proposta de exclusão de programa
	$sql = "select prgid from elabrev.proposta_exclusao_programa where peporigem<>'F' and prgid =".$_REQUEST['prgid']."  and usucpf='".$_SESSION['usucpf']."'";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas < 0)	
	   $sql = "insert into elabrev.proposta_exclusao_programa (prgid,usucpf,pepjustificativa, prsano) values (".$_REQUEST['prgid'].",'".$_SESSION['usucpf']."','".$_REQUEST['pepjustificativa']."','".$_SESSION['exercicio']."')";
	else
	  $sql = "update elabrev.proposta_exclusao_programa set pepjustificativa = '".$_REQUEST['pepjustificativa']."' where peporigem='N' and prgid=".$_REQUEST['prgid']." and usucpf='".$_SESSION['usucpf']."' ";
      $res = $db->executar($sql);
      $db->commit();    
      $db->sucesso($modulo,"&lista=E");
}


if ($_REQUEST['prog_n'] == 'excluipep' and $_REQUEST['codid'] >0 )
{
	// há proposta de exclusão da proposta de exclusao
	$sql = "delete from elabrev.proposta_exclusao_programa where pepid =".$_REQUEST['codid'];
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo,"&lista=E");
}

if ($_REQUEST['decisaoexclui'])
{
	// decidiu
	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.proposta_exclusao_programa set pepstatus='A', usucpfdecisor=null where prgid=".$_SESSION['prgid']." and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.proposta_exclusao_programa set pepstatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where pepid=".$_REQUEST['decisaoexclui']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.proposta_exclusao_programa set pepstatusspo='A', usucpfdecisorspo=null where prgid=".$_SESSION['prgid']." and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
		$sql = "update elabrev.proposta_exclusao_programa set pepstatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where pepid=".$_REQUEST['decisaoexclui']; 	 
  		$db->executar($sql);
	}
	$db->commit();
	?>
      <script>
         alert('Gravação com sucesso.')
        // window.close();
      </script>
    <?
 //exit();
}
if ($_REQUEST['cdecisaoexclui'])
{
	// cancelou a decisão
	$momento = $db->verifica_momento();
	if ($momento==1)
	$sql = "update elabrev.proposta_exclusao_programa set pepstatus='A',usucpfdecisor=null where pepid=".$_REQUEST['cdecisaoexclui']; 
	else if ($momento==2) $sql = "update elabrev.proposta_exclusao_programa set pepstatusspo='A',usucpfdecisorspo=null where pepid=".$_REQUEST['cdecisaoexclui']; 	
  	$db->executar($sql);
    $db->commit();
   ?>
      <script>
         alert('Gravação com sucesso.')
         //window.close();
      </script>
  <?
 // exit();
}

//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Proposta de Exclusão de Programa';
monta_titulo($titulo_modulo,'');
$sql= "select p.prgcod, p.prgdsc, p.prgid from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where p.prgstatus in ('A','N') and p.prsano = '".$_SESSION['exercicio']."'  and p.prgid=".$_SESSION['prgid']."   limit 1";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;


$momento = $db->verifica_momento();
$criadorep = $db->ehcriador($_SESSION['prgid'],'EP'); // criador da exclusão
$decisor = $db->ehdecisor($_SESSION['prgid'],'P',$momento); // verifica se é decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['prgid'],'P',$momento); // verifica se é digitador

?>
<title>Proposta de Exclusão de Programa</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?>
      </tr> 
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="prgcod" value="<?=$prgcod?>">
<input type=hidden name="decisaoexclui" value="0">
<input type=hidden name="cdecisaoexclui" value="0">
<input type=hidden name="codid" >
<input type=hidden name="prog_n" >
<center>

 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  
<?
    $proposexclui=0;
	$sql = "select pepjustificativa,pepid, pepstatus,pepstatusspo from elabrev.proposta_exclusao_programa where prgid =".$_REQUEST['prgid']."  and usucpf='".$_SESSION['usucpf']."' and peporigem='N' limit 1";

    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas ==0) {
    	$proposexclui=1;
    $res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    }
?>  

  <tr>
  <td colspan=2 align='right' class="SubTituloDireita"><b><center>Proposta de Exclusão de Programa</b>
  </td>
  </tr>  
  <tr>
  <td align='right' class="SubTituloDireita"><b>Justificativa:</b>
  </td>
        <td><?=campo_textarea('pepjustificativa','S','S','',80,5,'','dica','');        
         if ($proposexclui and 	$momento and $criadorep and $pepstatus=='A' and $pepstatusspo=='A')
        {
        	print '<input type="button" class="botao" name="btexclui" value= "Excluir a proposta" onclick="exclui_pep('.$pepid.')">&nbsp;&nbsp;&nbsp;';
        }
        ?>
        </td>  
  </tr>     
	 <?
	 if ($momento and $digit and $pepstatus=='A' and $pepstatusspo=='A')
	 {?>
        <tr ><td colspan="2" align="center">        
        <input type='button' class="botao" value='Gravar' onclick="gravar_janela('E')">
        </td></tr>
<?}?> 
  
 </table>
 <table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Propostas de Exclusão neste Programa</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Opções</strong></td>
      <td valign="top" class="title"><strong>Proponente</strong></td>     
    </tr>
</thead>
  <tbody>
   <?
	$sql="select p.pepid,p.pepstatus,p.pepstatusspo,p.usucpfdecisor, p.usucpfdecisorspo, p.usucpf,'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justifexclui(' || p.pepid|| ')\">' as verjustificativa, case when p.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || p.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.proposta_exclusao_programa p inner join seguranca.usuario u on p.usucpf=u.usucpf  left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.peporigem<>'F' and p.pepstatus in ('A','D') and  p.prgid=".$_SESSION['prgid']." and p.prsano='".$_SESSION['exercicio']."' order by p.pepstatus desc";
	$rs = @$db->carregar( $sql );
    if (  $rs && count($rs) > 0 )
    { 
	   $m=-1;
       foreach ( $rs as $linha )
       {
  	    $m++;
	    foreach($linha as $k=>$v) ${$k}=$v;
	    if (fmod($m,2) == 0) $marcado = '' ; else $marcado='marcado';
   		$decisao='';
		if ($momento==1)
	    {
			// então está dentro do momento
			// verifico se já possui decisão
			if ($pepstatus=='D')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decisão!' onclick=\"cancelar_decisaoexc('".$pepid."')\">";
				}
				else 
				{			
					// neste caso, tem decisão mas não é quem decidiu, logo só pode ver
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Proposta já decidida por $decisoruo!' >";            
				}
			}
			else
			{
				// então ainda não tem decisão
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisaoexc('".$pepid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// então está dentro do momento SPO
			// verifico se já possui decisão
			if ($pepstatus=='D')
			{
				// então alguém já decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta já decidida nível UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($pepstatusspo=='S')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($db->ehcoorduma($usucpfdecisorspo)) $valid='valida6.gif' ; else $valid='valida4.gif';
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao3="<img border='0' src='../imagens/$valid' title=' Cancelar a decisão nível SPO!' onclick=\"cancelar_decisaoexc('".$pepid."')\">";
				}
				else 
				{			
					// neste caso, tem decisão mas não é quem decidiu, logo só pode ver
					$decisao3="<img border='0' src='../imagens/$valid' title=' Proposta já decidida na SPO por $decisorspo!' >";            
				}
			}
			else
			{
				// então ainda não tem decisão
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta nível SPO!' onclick=\"tomar_decisaoexc('".$pepid."')\">";
				}
			}
		}
	print '<tr class="'.$marcado.'"><td>'.$verjustificativa.'&nbsp;&nbsp;'.$decisao.$decisao2.$decisao3.'</td><td>'.$autor.'</td></tr>';
  }
}
else
{
  print '<tr class="'.$marcado.'"><td>Não há registros de propostas de exclusão</td></tr>';
}
?>
   </tbody>
    </table>

</form>

<script>
 function gravar_janela(cod)
   {
   	   document.formulario.prog_n.value=cod;
   	   document.formulario.submit();
   }
   
 function exclui_pep(cod)
   {
   	if( window.confirm( "Confirma a exclusão ?" ))
    {
   	  	document.formulario.prog_n.value='excluipep';
   	  	document.formulario.codid.value=cod;
   	  	document.formulario.submit();
    }
   }  
   
  function envia_email(cpf)
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
       window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
  
  function ver_justifexclui(cod)
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justifmigra&acao=A&cod="+cod+"&exc=2";          
       window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    
  function tomar_decisaoexc(cod)
    {
    	if( window.confirm( "Confirma a decisão?") )
         {
	         document.formulario.decisaoexclui.value = cod;
	         document.formulario.submit();
         } else return;
    }   
  
  function cancelar_decisaoexc(cod)
    {
    	if( window.confirm( "Confirma o CANCELAMENTO da decisão?") )
         {
	         document.formulario.cdecisaoexclui.value = cod;
	         document.formulario.submit();
         } else return;
    }   
</script>

