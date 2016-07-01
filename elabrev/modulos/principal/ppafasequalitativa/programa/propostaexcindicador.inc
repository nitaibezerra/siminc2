<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:novoprograma.inc
   Finalidade: permitir a inclusão de um novo programa
   */
$modulo=$_REQUEST['modulo'] ;//
			$prgid = (integer) $_SESSION['prgid'];
$indnum = md5_decrypt($_REQUEST['indnum'],'');

if ($_REQUEST['indnum'] and $_REQUEST['peijustificativa'] )
{
	// há proposta de exclusão de indicador
	$sql = "select indnum from elabrev.proposta_exclusao_indicador where indnum =".$indnum."  and usucpf='".$_SESSION['usucpf']."'";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas < 0)	
	   $sql = "insert into elabrev.proposta_exclusao_indicador (prgid,usucpf,peijustificativa, prsano) values (".$indnum.",'".$_SESSION['usucpf']."','".$_REQUEST['peijustificativa']."','".$_SESSION['exercicio']."')";
	else
	  $sql = "update elabrev.proposta_exclusao_indicador set peijustificativa = '".$_REQUEST['peijustificativa']."' where indnum=".$indnum." and usucpf='".$_SESSION['usucpf']."' ";
      $res = $db->executar($sql);
      $db->commit();    
      $db->sucesso($modulo,"&lista=E");
}


if ($_REQUEST['prog_i'] == 'excluipei' and $_REQUEST['codid'] >0 )
{
	// há proposta de exclusão 
	$sql = "delete from elabrev.proposta_exclusao_indicador where peiid =".$_REQUEST['codid'];
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);
}

if ($_REQUEST['decisaoexclui'])
{
	// decidiu
	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.proposta_exclusao_indicador set peistatus='A', usucpfdecisor=null where indnum=".$indnum." and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.proposta_exclusao_indicador set peistatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where peiid=".$_REQUEST['decisaoexclui']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.proposta_exclusao_indicador set peistatusspo='A', usucpfdecisorspo=null where indnum=".$indnum." and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
		$sql = "update elabrev.proposta_exclusao_indicador set peistatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where peiid=".$_REQUEST['decisaoexclui']; 	 
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
	$sql = "update elabrev.proposta_exclusao_indicador set peistatus='A',usucpfdecisor=null where peiid=".$_REQUEST['cdecisaoexclui']; 
	else if ($momento==2) $sql = "update elabrev.proposta_exclusao_indicador set peistatusspo='A',usucpfdecisorspo=null where peiid=".$_REQUEST['cdecisaoexclui']; 	
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

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Proposta de Exclusão de Indicador';
monta_titulo($titulo_modulo,'');
$sql= "select p.prgcod, p.prgdsc, p.prgid from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where p.prgstatus='A' and p.prsano = '".$_SESSION['exercicio']."'  and p.prgid=".$_SESSION['prgid']."   limit 1";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$acacod1 = $acacod;
$prgcod1 = $prgcod;

$momento = $db->verifica_momento();
$criadorep = $db->ehcriador($_SESSION['prgid'],'EP'); // criador da exclusão
$decisor = $db->ehdecisor($_SESSION['prgid'],'P',$momento); // verifica se é decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['prgid'],'P',$momento); // verifica se é digitador

?>

<head>
<title>Proposta de Revisão</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="decisaoexclui" value="0">
<input type=hidden name="cdecisaoexclui" value="0">
<input type=hidden name="codid" >
<input type=hidden name="indnum" value="<?=md5_encrypt($indnum,'')?>" >


<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=campo_textarea('peijustificativa','S','S','',80,3,'','','','');?></td>       
      </tr>
   <?
  if ($momento)
  {?>
    
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_janela()"></tr>
      
  <?}?>
    </table>
  
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Propostas de Exclusão neste Indicador</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Opções</strong></td>
      <td valign="top" class="title"><strong>Proponente</strong></td>     
    </tr>
</thead>
  <tbody>
   <?
	$sql="select p.peiid,p.peistatus,p.peistatusspo,p.usucpfdecisor, p.usucpfdecisorspo, p.usucpf,'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justifexclui(' || p.peiid|| ')\">' as verjustificativa, case when p.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || p.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.proposta_exclusao_indicador p inner join seguranca.usuario u on p.usucpf=u.usucpf  left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.peiorigem<>'F' and p.peistatus in ('A','D') and  p.indnum=".$indnum." and p.prsano='".$_SESSION['exercicio']."' order by p.peistatus desc";
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
			if ($peistatus=='D')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decisão!' onclick=\"cancelar_decisaoexc('".$peiid."')\">";
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
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisaoexc('".$peiid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// então está dentro do momento SPO
			// verifico se já possui decisão
			if ($peistatus=='D')
			{
				// então alguém já decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta já decidida nível UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($peistatusspo=='S')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao3="<img border='0' src='../imagens/valida4.gif' title=' Cancelar a decisão nível SPO!' onclick=\"cancelar_decisaoexc('".$peiid."')\">";
				}
				else 
				{			
					// neste caso, tem decisão mas não é quem decidiu, logo só pode ver
					$decisao3="<img border='0' src='../imagens/valida4.gif' title=' Proposta já decidida na SPO por $decisorspo!' >";            
				}
			}
			else
			{
				// então ainda não tem decisão
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta nível SPO!' onclick=\"tomar_decisaoexc('".$peiid."')\">";
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
   	  //prepara_formulario();
   	  // testar todos os campos antes de incluir

   	  if (!validaBranco(document.formulario.peijustificativa, 'Justificativa')) return;
  	   document.formulario.submit();
   }

    
   function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
      
</script>

