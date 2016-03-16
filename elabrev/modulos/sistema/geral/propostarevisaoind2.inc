<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:propostafusao_inclusao.inc
   Finalidade: permitir a proposta de fusao em uma inclusão
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];

$titulo_modulo='Proposta de Revisão de Atributos';
monta_titulo($titulo_modulo,'');

$sql= "select p.*  from elabrev.ppaprograma_proposta p  where p.prgstatus in ('A','N') and p.prgid = ".$_REQUEST['prgid'];

if (! $_REQUEST['prgid'] or $_REQUEST['prgid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a página. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}


$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

if ($nlinhas < 0 ){
	?>
	<script>alert('Programa Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  $db->carrega_registro($RS,0);

}
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;


if ($_REQUEST['decisao'])
{
	// decidiu
	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatus='A', usucpfdecisor=null where eracod=".$_SESSION['prgid']." and eracampo='".$_REQUEST['campo']."' and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where eraid=".$_REQUEST['decisao']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatusspo='A', usucpfdecisorspo=null where eracod=".$_SESSION['prgid']." and eracampo='".$_REQUEST['campo']."' and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where eraid=".$_REQUEST['decisao']; 
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


if ($_REQUEST['cdecisao'])
{
	// cancelou a decisão
	$momento = $db->verifica_momento();
	if ($momento==1)
	$sql = "update elabrev.elaboracaorevisao set eradata=current_date, erastatus='A', usucpfdecisor=null where eraid=".$_REQUEST['cdecisao']; 
	else if ($momento==2) $sql = "update elabrev.elaboracaorevisao set eradata=current_date, erastatusspo='A',usucpfdecisorspo=null  where eraid=".$_REQUEST['cdecisao'];	
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


if ($_REQUEST['grava'] and $_REQUEST['erajustificativa'])      
{
  if ($_REQUEST['origem']<>$_REQUEST['alvo'])
  {
  	// então as tabelas de origem e alvo são diferentes. Tratá-se de uma foreing key
  	if ($_REQUEST['padrao']<>'tpr')
  	$sqldsc = "select ".$_REQUEST['padrao']."dsc from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."cod=".$_REQUEST['erapropostacod'];
  	else $sqldsc = "select ".$_REQUEST['padrao']."nome from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."cod=".$_REQUEST['erapropostacod'];  	
    $titulo=$db->recuperar($sqldsc);
    $_REQUEST['eraproposta'] = $titulo[$_REQUEST['padrao'].'dsc'];
    if ($_REQUEST['padrao']=='tpr') $_REQUEST['eraproposta'] = $titulo[$_REQUEST['padrao'].'nome'];
  }
  $alvo=$_REQUEST['alvo'];
  $origem=$_REQUEST['origem'];
  if ($_REQUEST['inserir']==1)
  {

  	if (! $_REQUEST['erapropostacod']) $_REQUEST['erapropostacod']=0;
  	$sql = "insert into elabrev.elaboracaorevisao (
  	  usucpf,
      eratabela,
      eracampo,
      prsano,
      eraproposta,
      erajustificativa,
      eracod,
      erapropostacod ) values ('".
  	$_SESSION['usucpf']."','$origem','".
  	$_REQUEST['campo']."','".
  	$_SESSION['exercicio']."','".
  	$_REQUEST['eraproposta']."','".
  	$_REQUEST['erajustificativa']."',".
  	$_SESSION['prgid'].",".
  	$_REQUEST['erapropostacod'].")";

  	 $db->executar($sql);
    $db->commit();
  //exit();
  }
    if ($_REQUEST['inserir']==2)
  {
  	$sql = "update elabrev.elaboracaorevisao set eradata=now() ,eraproposta='".$_REQUEST['eraproposta']."',erapropostacod='".$_REQUEST['erapropostacod']."',erajustificativa='".$_REQUEST['erajustificativa']."' where usucpf='".$_SESSION['usucpf']."' and eratabela='$origem' and eracampo='".$_REQUEST['campo']."' and prsano='".$_SESSION['exercicio']."' ";
  	 $db->executar($sql);
	 $db->commit();
	 ?>
      <script>
         alert('Gravação com sucesso.')
        // window.close();
      </script>
  <?
  //exit();
	}
	$_REQUEST['grava']=0;
}
 if ($_REQUEST['tipo_obj']=='combo')
 {
 	if ($_REQUEST['campo']=='tprcod' or $_REQUEST['padrao']=='tpr') 
 	$sql="select ".$_REQUEST['padrao']."nome as texto from ".$_REQUEST['alvo']." where ".$_REQUEST['campo']."=".$_REQUEST['especial'];
 	else $sql="select ".$_REQUEST['padrao']."dsc as texto from ".$_REQUEST['alvo']." where ".$_REQUEST['campo']."=".$_REQUEST['especial'];

 }
  else if ($_REQUEST['tipo_obj']=='bool')
 {
 	if ($_REQUEST['especial']=='f') $texto='Não'; else $texto='Sim';
 }
 else  
 {
 	 $sql="select ".$_REQUEST['campo']." as texto from ".$_REQUEST['origem']." where prgid=".$_SESSION['prgid'];
 }

  if (! $texto) $texto = @$db->pegaUm($sql);
 if (! $texto) $texto='Sem valor atual';
  
    $sql="select p.eraid,p.erapropostacod, p.eraproposta,p.usucpf,p.erastatus,p.erastatusspo, p.erajustificativa from elabrev.ElaboracaoRevisao p inner join seguranca.usuario u on u.usucpf='".$_SESSION['usucpf']."' and p.usucpf=u.usucpf where p.eratabela='".$_REQUEST['origem']."' and p.eracod=".$_SESSION['prgid']."  and p.prsano='".$_SESSION['exercicio']."' and p.eracampo='".$_REQUEST['campo']."'";
    $RS = $db->record_set($sql);   
    $nlinhas= $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {
       $res =  $db->carrega_registro($RS,0);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       $criadorpa = $db->ehcriador($eraid,'CP'); // criador da proposta ? 
    }
 
    $sql="select prgid from elabrev.ppaprograma_proposta where prgid=".$_SESSION['prgid']."  and prsano='".$_SESSION['exercicio']."' limit 1";
 
    $RS = $db->record_set($sql);
 
    $nlinhas= $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {
   		$res =  $db->carrega_registro($RS,0);
    	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 
    }  

$momento = $db->verifica_momento();
$decisor = $db->ehdecisor($_SESSION['prgid'],'P',$momento); // verifica se é decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['prgid'],'P',$momento); // verifica se é digitador
?>

<html>
<head>
<title>Proposta de Revisão</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="origem" value="<?=$_REQUEST['origem']?>">
<input type=hidden name="alvo" value="<?=$_REQUEST['alvo']?>">
<input type=hidden name="campo" value="<?=$_REQUEST['campo']?>">
<input type=hidden name="decisao" value="0">
<input type=hidden name="cdecisao" value="0">
<input type=hidden name="grava" value="0">
<input type=hidden name="tipo_obj" value="<?=$_REQUEST['tipo_obj']?>">
<? if ($eraproposta) {?>
<input type=hidden name="inserir" value="2">
<?} else {?>
<input type=hidden name="inserir" value="1">
<?}
  $v_explicacao=$db->busca_explicacao($_REQUEST['campo'],$_REQUEST['origem']);
  $explicacao = $v_explicacao['explicacao'];
  $tmax=$v_explicacao['explicacao'];
  $tipo=$v_explicacao['tipo'];

?>
  <center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right' class="subtitulodireita">Programa:</td>
        <td><?=$prgcod.'.'.$prgdsc?></td>
      </tr>
  
	  <tr>
        <td align="right" class="subtitulodireita">Atributo a ser alterado:</td> 
        <td ><b><?=$_REQUEST['nomcampo'];?></b></td>       
     </tr>  
     
	  <tr>
        <td align="right" class="subtitulodireita">Conteúdo atual:</td> 
        <td ><?=$texto;?></td>       
     </tr>      
 	  <tr>
        <td align="right" class="subtitulodireita">Proposta:</td>
        <td>
        <? if ($_REQUEST['tipo_obj']=='combo') 
        {
        	if ($_REQUEST['campo']=='tprcod')        	
        	{
        		$sql = "select ".$_REQUEST['padrao']."cod as codigo, ".$_REQUEST['padrao']."nome as descricao from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."status='A' ";
        		if ($_REQUEST['especial']) $sql .= ' and '.$_REQUEST['padrao'].'cod not in ('.$_REQUEST['especial'].')';
        		if ($_REQUEST['especial2']) $sql .= ' and  ('.$_REQUEST['especial2'].')';  					$sql .= " order by ".$_REQUEST['padrao']."nome";
        	}
        	else 
        	{
        		$sql = "select ".$_REQUEST['padrao']."cod as codigo, ".$_REQUEST['padrao']."dsc as descricao from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."status='A' ";
        		if ($_REQUEST['especial']) $sql .= ' and '.$_REQUEST['padrao'].'cod not in ('.$_REQUEST['especial'].')';
        		if ($_REQUEST['especial2']) $sql .= ' and  ('.$_REQUEST['especial2'].')';
      				$sql .= " order by ".$_REQUEST['padrao']."dsc";
			}
        	print $db->monta_combo("erapropostacod",$sql,'S',"Escolha uma opção",'','',$explicacao,300);
        
        } 
        else if ($_REQUEST['tipo_obj']=='text')
        {
        	print campo_textarea('eraproposta','N','S','','80',5,'','','',$explicacao);} 
        else if ($_REQUEST['tipo_obj']=='texto')
        {
            print campo_texto('eraproposta','S','S','',80,$tmax,'','','', $explicacao,'');   }  
        else if ($_REQUEST['tipo_obj']=='numfrac')
        {
           print campo_texto('eraproposta','S','S','',10,$tmax,'##########.##','','', $explicacao,'');
        }  
        else if ($_REQUEST['tipo_obj']=='numint')
        {
          print campo_texto('eraproposta','S','S','',10,$tmax,'##########','','', $explicacao,'');
         }  
         else if ($_REQUEST['tipo_obj']=='bool')
         {
         	print campo_texto('eraproposta','S','S','',20,$tmax,'##########','','', $explicacao,'');                                  	
        }   
         else if ($_REQUEST['tipo_obj']=='data'){
        	print '<script language="JavaScript" src="../includes/calendario.js"></script>';
            print campo_data('eraproposta','S','S','','S','','','','', $explicacao,'');  
        }      
        ?>
        </td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Justificativa:</td> 
        <td ><?=campo_textarea('erajustificativa','S','S','','80',5,'');?></td>         
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <?
	 if ($momento and $digit and $erastatus<>'D' and $erastatusspo<>'S')
	 {?>
	<input type='button' class="botao" value='OK!' onclick="grava_proposta()">&nbsp;&nbsp;&nbsp;
<?}?> <input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Ação</strong></td>
      <td valign="top" class="title"><strong>Autor</strong></td>
      <td valign="top" class="title"><strong>Proposta</strong></td>
    </tr>
</thead>
<tbody>
<?
$sql="select p.eraid,p.erastatus,p.erastatusspo, p.usucpf,p.usucpfdecisor,p.usucpfdecisorspo, '<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.eraid|| ')\">' as verjustificativa,p.eraproposta ||'<br> Digitado em: '||to_char(p.eradata,'dd/mm/yyyy') as proposta,
 case when u.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.ElaboracaoRevisao p inner join seguranca.usuario u on p.usucpf=u.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.erastatus in ('A','D') and  p.eratabela='".$_REQUEST['origem']."' and p.eracod=".$_SESSION['prgid']." and p.prsano='".$_SESSION['exercicio']."' and p.eracampo='".$_REQUEST['campo']."' order by p.erastatus desc,p.eradata desc";

$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0) {
for ($i=0; $i<=$nlinhas;$i++){
    $res = $db->carrega_registro($RS2,$i);
    // a linha abaixo transforma em variáveis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    $criadorpa = $db->ehcriador($eraid,'CP'); // criador da proposta ?
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
    $decisao='';
		if ($momento==1)
	    {
			// então está dentro do momento
			// verifico se já possui decisão
			if ($erastatus=='D')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decisão!' onclick=\"cancelar_decisao('".$eraid."')\">";
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
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisao('".$eraid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// então está dentro do momento SPO
			// verifico se já possui decisão
			if ($erastatus=='D')
			{
				// então alguém já decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta já decidida nível UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($erastatusspo=='S')
			{
				// então alguém já decidiu
				// verifico se o decisor é o mesmo que está logado. se for, então pode cancelar a decisão
				if ($db->ehcoorduma($usucpfdecisorspo)) $valid='valida6.gif' ; else $valid='valida4.gif';
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// então tem pleno acesso
					$decisao3="<img border='0' src='../imagens/$valid' title=' Cancelar a decisão nível SPO!' onclick=\"cancelar_decisao('".$eraid."')\">";
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
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta nível SPO!' onclick=\"tomar_decisao('".$eraid."')\">";
				}
			}
		}		
	print '<tr class="'.$marcado.'"><td>'.$verjustificativa.'&nbsp;&nbsp;'.$decisao.$decisao2.$decisao3.'</td><td>'.$autor.'</td><td>'.$proposta.'</td></tr>';

  }
}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de propostas de alteração</td></tr>';
}
?>
</tbody>
    </table>

<script>
  function fechar_janela()
  {
    window.close();

  }
    function grava_proposta()
  {
  	if (!validaBranco(document.formulario.erajustificativa, 'Justificativa')) return;
   	try
	{
   	if (!validaBranco(document.formulario.erapropostacod, 'Proposta')) return;
	} 
	catch(e)
	{
		if (!validaBranco(document.formulario.eraproposta, 'Proposta')) return; 
	}
	document.formulario.grava.value = 1;
	document.formulario.submit();
  }
      function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

      function ver_justificativa(cod)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justificativa&acao=A&cod="+cod;          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }    

     function tomar_decisao(cod)
    {
    	if( window.confirm( "Confirma a decisão?") )
         {
	         document.formulario.decisao.value = cod;
	         document.formulario.submit();
         } else return;
    }  
      
     function cancelar_decisao(cod)
    {
    	if( window.confirm( "Confirma o CANCELAMENTO da decisão?") )
         {
	         document.formulario.cdecisao.value = cod;
	         document.formulario.submit();
         } else return;
    }
        
</script>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
