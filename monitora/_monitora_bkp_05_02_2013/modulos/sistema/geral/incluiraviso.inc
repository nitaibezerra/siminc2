<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:incluiraviso.inc
   Finalidade: permitir escrever um aviso
   */
$modulo=$_REQUEST['modulo'];

function atualiza_interessado($id, $usucpfs,$msgid )
	{
		global $db;
		$id = (integer) $id;
		if ( (!$id) or (! is_array($usucpfs)))
		{
			return;
		}
		$sql="select avsdescricao as assunto2, u2.ususexo as sexolancador,u2.usunome as nomelancador, u2.usufoneddd||'-'||u2.usufonenum as fonelancador from aviso av inner join seguranca.usuario u2 on u2.usucpf=av.usucpf where avsid=$id";

		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		
		// cria a mensagem se ela ainda não foi criada

		$sql_remocao = "delete from aviso_interessado where usucpf is not null and avsid=".$id;
		$db->executar( $sql_remocao );
		foreach ( (array) $usucpfs as $usucpf )
		{
			if ($usucpf) {
				$sql_insercao = "insert into aviso_interessado (avsid, usucpf ) values ( " . $id . ",'" . $usucpf . "')";	
				$db->executar( $sql_insercao );
			// cria mensagem na caixa de mensagem do interessado
			$sql = "insert into mensagemusuario (msgid,usucpf) values ($msgid,'$usucpf')";
			$saida=$db->executar($sql);
		 	
			// envia email para o interessado
			
        $assunto = 'SIMEC- Aviso importante!';
		$sexo = 'Prezado Sr.  ';
		if ($sexolancador=='F') $sexolanc='Sra. '; else $sexolanc='Sr. '; 
		$sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$usucpf."'";
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		
        $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi inserido um aviso no Simec com o seguinte texto:\n $assunto2. \n Qualquer dúvida favor consultar o sistema SIMEC ou entrar em contato com:\n $sexolanc $nomelancador (tel:$fonelancador) E-mail:$emaillancador";
        email($usunome, $usuemail, $assunto, $mensagem);
			}

		}

	}   

$campo=$_REQUEST['campo'];
if ($campo=='pe') $campo='pjeid';
if ($_REQUEST['id']) $id=$_REQUEST['id'];
$botao='Criar Aviso';
$acao="cria_registro()";
if ($_REQUEST['avsid']) 
{
	// então está editando o registro
	$sql = "select av.*, ap.* from aviso av inner join aviso_processo ap on ap.avsid=av.avsid where av.avsid=".$_REQUEST['avsid'];
	$linha=$db->pegaLinha($sql);
	foreach($linha as $k=>$v) ${$k}=$v;
	$id=${$campo};
	$botao='Alterar Aviso';
	$acao="altera_registro($avsid)";
}



if ($_REQUEST['act']=='incluir')
{
	// inclusão de item de aviso
   $sql = "insert into aviso (avsdescricao,avsvalidade,usucpf,avsdataini) values (".
   "'".($_REQUEST['avsdescricao'])."',".
   $_REQUEST['avsvalidade'].",".
   "'".$_SESSION['usucpf']."','".$_REQUEST['avsdataini']."')";
    $saida = $db->executar($sql);
    $sql =  "select avsid from aviso where oid = ".pg_last_oid($saida);
    $avsid = $db->pegaUm($sql);
    
    // cria a mensagem
    $sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Aviso importante!','".$_REQUEST['avsdescricao']."','".$_SESSION['usucpf']."',".$_SESSION['sisid'].")";
	$saida=$db->executar($sql);
    $sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);	
    $ultimomsgid=$db->pegaUm($sql);

       atualiza_interessado($avsid, $_REQUEST['usucpf'],$ultimomsgid );
 
       // cria registro em aviso_processo
       $sql ="insert into aviso_processo ($campo,avsid) values ($id,$avsid)";
       $saida = $db->executar($sql);       
       $db->commit();	
       $db->close();
       ?>
       <script>
           window.close();
           opener.location.reload();
       </script>
       <?
       exit();
       
}

if ($_REQUEST['act']=='alterar')
{
	// alteração de aviso
	    $avsid = $_REQUEST['id2'];
        $sql = "update aviso set avsdescricao='".$_REQUEST['avsdescricao']."',avsvalidade=".$_REQUEST['avsvalidade'].",usucpf='".$_SESSION['usucpf']."' ,avsdataini='".$_REQUEST['avsdataini']."' where avsid=$avsid";
       $saida = $db->executar($sql);
	  // cria a mensagem
    $sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Alteração de Aviso!','".$_REQUEST['avsdescricao']."','".$_SESSION['usucpf']."',".$_SESSION['sisid'].")";
	$saida=$db->executar($sql);
    $sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);	
    $ultimomsgid=$db->pegaUm($sql);
     
       atualiza_interessado($avsid, $_REQUEST['usucpf'],$ultimomsgid );
 
           
       $db->commit();	
       $db->close();
       ?>
       <script>
           window.close();
           opener.location.reload();
       </script>
       <?
       exit();
          
       
}


if ($campo=='pjeid')
{
	$sql_usuario_av = "select distinct u.usucpf as codigo, u.usunome as descricao 
from seguranca.usuario u inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.suscod='A' and us.sisid=".$_SESSION['sisid']." inner join monitora.projeto_ator pa on u.usucpf=pa.usucpf and pa.peastatus='A' and pa.pjeid=$id where u.suscod='A' order by descricao";	
	//print $sql_usuario_av ;
}


?>
<html>
<head>
<title>Marcação de Reunião</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>

</head>

<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="id" value="<?=$id?>">
<input type=hidden name="campo" value="<?=$_REQUEST['campo']?>">
<input type=hidden name="act" value="0">
<input type=hidden name="id2" value="<?=$avsid?>">

<br>
 <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
	 <tr>
        <td align="right" class="subtitulodireita">Data inicial:</td> 
        <td><?=campo_data('avsdataini', 'S','S','','S');?></td>
     </tr>      
     <tr>
        <td align="right" class="subtitulodireita">Validade:</td> 
        <td><?=campo_texto('avsvalidade', 'S','S','Entre com a validade em dias',3,3,"##",'');?></td>
     </tr> 
 
	<tr>
        <td align='right' class="SubTituloDireita">Interessados:</td>
        <td>
        	<?
        	$sql_auxiliar_av = $_REQUEST[ 'avsid' ] ? "select distinct u.usucpf as codigo, u.usunome as descricao from aviso_interessado arp inner join seguranca.usuario u on arp.usucpf=u.usucpf  inner join aviso ar on ar.avsid=arp.avsid inner join aviso_processo ap on ap.avsid=ar.avsid and ap.$campo=$id where arp.usucpf=u.usucpf order by descricao" : '';        	
            $usucpf = $_REQUEST[ 'avsid' ] ? $db->carregar( $sql_auxiliar_av ) : ''; 
       		combo_popup( 'usucpf', $sql_usuario_av, 'Selecione o(s) Participante(s)', '360x460' )?>          </td>      
      </tr>                
     
     <tr align="center">
        <td colspan="2"><b>AVISO</td> 
     </tr>          
     <tr> 
        <? $agrpauta= '';?>
        <td colspan=2><?=campo_textarea('avsdescricao','N','S','','95%',13,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='<?=$botao?>' onclick="<?=$acao?>">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 
<script>
  function fechar_janela()
  {
    window.close();

  }

  function cria_registro()
  {
    prepara_formulario();
        if (!validaBranco(document.formulario.avsdataini, 'Data inicial')) return;
  	if (! validaDataMaiorQueHoje(document.formulario.avsdataini))
		{
			alert("Data não pode ser Anterior que Data atual.");
			document.formulario.avsdataini.focus();
			return;
		}         
  	if (!validaBranco(document.formulario.avsvalidade, 'Validade do aviso')) return;
	//verificação do campo corpo email
	if (!validaBranco(document.formulario.avsdescricao, 'Aviso')) return tinyMCE.execCommand('mceFocus', true, 'avsdescricao');
	document.formulario.act.value='incluir'	;
	document.formulario.submit();

  }
  function altera_registro(cod)
  {
    prepara_formulario();
    if (!validaBranco(document.formulario.avsdataini, 'Data inicial')) return;
  	if (! validaDataMaiorQueHoje(document.formulario.avsdataini))
		{
			alert("Data não pode ser Anterior que Data atual.");
			document.formulario.avsdataini.focus();
			return;
		}    
  	if (!validaBranco(document.formulario.avsvalidade, 'Validade do aviso')) return;
	//verificação do campo corpo email
	if (!validaBranco(document.formulario.avsdescricao, 'Aviso')) return tinyMCE.execCommand('mceFocus', true, 'avsdescricao');
	document.formulario.act.value='alterar'	;
	document.formulario.submit();

  }  

</script>
</body>
</html>
