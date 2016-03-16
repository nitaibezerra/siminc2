<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:incluireuniao.inc
   Finalidade: permitir incluir evento de reunião
   */
$modulo=$_REQUEST['modulo'];

function atualiza_participante($id, $usucpfs,$msgid )
	{
		global $db;
		$id = (integer) $id;
		if ( (!$id) or (! is_array($usucpfs)))
		{
			return;
		}
		$sql="select agrassunto as assunto, agrlocal as local,agrdata as data, agrhora as hora,agrduracao as duracao,  agrpauta as pauta, u2.ususexo as sexolancador,u2.usunome as nomelancador, u2.usufoneddd||'-'||u2.usufonenum as fonelancador, u2.usuemail as emaillancador from agenda_reuniao ar inner join seguranca.usuario u2 on u2.usucpf=ar.usucpf where agrid=$id";

		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		
		$sql_remocao = "delete from agenda_reuniao_participante where usucpf is not null and agrid=".$id;
		$db->executar( $sql_remocao );
		foreach ( (array) $usucpfs as $usucpf )
		{
			if ($usucpf) {
				$sql_insercao = "insert into agenda_reuniao_participante (agrid, usucpf ) values ( " . $id . ",'" . $usucpf . "')";	
			$db->executar( $sql_insercao );
			// cria mensagem na caixa de mensagem do interessado
			$sql = "insert into mensagemusuario (msgid,usucpf) values ($msgid,'$usucpf')";
			$saida=$db->executar($sql);
			
			// envia email para o interessado
			
        $assunto = 'SIMEC- Marcação de reunião';
		$sexo = 'Prezado Sr.  ';
		if ($sexolancador=='F') $sexolanc='Sra. '; else $sexolanc='Sr. '; 
		$sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$usucpf."'";
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		
        $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi marcada reunião com os seguintes dados:\n Assunto:$assunto \n Data/Hora:$data/$hora \n Duração prevista:$duracao \n Local:$local \n Pauta:$pauta. \n Qualquer dúvida favor consultar o sistema SIMEC ou entrar em contato com:\n $sexolanc $nomelancador (tel:$fonelancador) E-mail:$emaillancador";
        if (! $_REQUEST['datavencida'])   email($usunome, $usuemail, $assunto, $mensagem);
			}

		}

	}   


$campo=$_REQUEST['campo'];
if ($campo=='pe') $campo='pjeid';
if ($_REQUEST['id']) $id=$_REQUEST['id'];
$botao='Criar item na agenda';
$acao="cria_registro()";
if ($_REQUEST['agrid']) 
{
	// então está editando o registro
	$sql = "select ar.*, arp.* from agenda_reuniao ar inner join agenda_reuniao_processo arp on arp.agrid=ar.agrid where ar.agrid=".$_REQUEST['agrid'];
	$linha=$db->pegaLinha($sql);
	foreach($linha as $k=>$v) ${$k}=$v;
	$id=${$campo};
	$botao='Alterar item na agenda';
	$acao="altera_registro($agrid)";
}

if ($_REQUEST['act']=='incluir')
{
	// inclusão de item na agenda
	$agrassunto= str_to_upper($_REQUEST['agrassunto']);
    $agrlocal=str_to_upper($_REQUEST['agrlocal']);
    $agrdata=$_REQUEST['agrdata'];
    $agrhora=$_REQUEST['agrhora'];
    $agrduracao= $_REQUEST['agrduracao'];
    $agrpauta = $_REQUEST['agrpauta'];
    $usucpf=$_SESSION['usucpf'];
    
    $sql = "insert into agenda_reuniao (agrassunto,agrlocal,agrdata,agrhora,agrduracao,agrpauta,usucpf) values ('$agrassunto','$agrlocal','$agrdata','$agrhora','$agrduracao','$agrpauta','$usucpf')";
    $saida = $db->executar($sql);
    $sql =  "select agrid from agenda_reuniao where oid = ".pg_last_oid($saida);
    $agrid = $db->pegaUm($sql);
    $sql="select usunome, usufoneddd||'-'||usufonenum as fone,usuemail from seguranca.usuario where usucpf='$usucpf'";
    $res=$db->pegaLinha($sql);

    foreach($res as $k=>$v) ${$k}=$v;

    // cria a mensagem
    $sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Agendamento de reunião!',
    'Reportamos que foi marcada reunião com os seguintes dados:<br>Assunto: $agrassunto<br>Data/Hora: $agrdata/$agrhora<br>Duração prevista: $agrduracao<br>Local: $agrlocal<br>Pauta: $agrpauta<br>Qualquer dúvida favor consultar o sistema SIMEC ou entrar em contato com: $usunome ($fone) ou e-mail: $usuemail','$usucpf',".$_SESSION['sisid'].")";

	$saida=$db->executar($sql);
    $sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);	
    $ultimomsgid=$db->pegaUm($sql);
    atualiza_participante($agrid, $_REQUEST['usucpf'],$ultimomsgid);
 
       // cria registro em agenda_processo
       $sql ="insert into agenda_reuniao_processo ($campo,agrid) values ($id,$agrid)";
      
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
	// alteraçãoo de item na agenda
	  	$agrassunto= str_to_upper($_REQUEST['agrassunto']);
    $agrlocal=str_to_upper($_REQUEST['agrlocal']);
    $agrdata=$_REQUEST['agrdata'];
    $agrhora=$_REQUEST['agrhora'];
    $agrduracao= $_REQUEST['agrduracao'];
    $agrpauta = $_REQUEST['agrpauta'];
    $usucpf=$_SESSION['usucpf'];
    
    $sql = "update agenda_reuniao set agrassunto='$agrassunto',agrlocal='$agrlocal',agrdata='$agrdata',agrhora='$agrhora',agrduracao='$agrduracao',agrpauta='$agrpauta',usucpf='$usucpf' where agrid=".$_REQUEST['id2'];
    $saida = $db->executar($sql);
    $agrid = $_REQUEST['id2'];
    $sql="select usunome, usufoneddd||'-'||usufonenum as fone,usuemail from seguranca.usuario where usucpf='$usucpf'";
    $res=$db->pegaLinha($sql);
    foreach($res as $k=>$v) ${$k}=$v;

// cria a mensagem
    $sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Alteração de reunião agendada!',
    'Reportamos que foi alterada a reunião agendada com os novos dados:<br>Assunto: $agrassunto<br>Data/Hora: $agrdata/$agrhora<br>Duração prevista: $agrduracao<br>Local: $agrlocal<br>Pauta: $agrpauta<br>Qualquer dúvida favor consultar o sistema SIMEC ou entrar em contato com: $usunome ($fone) ou e-mail: $usuemail','".$_SESSION['usucpf']."',".$_SESSION['sisid'].")";
 
	$saida=$db->executar($sql);
    $sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);	
    $ultimomsgid=$db->pegaUm($sql);
    atualiza_participante($agrid, $_REQUEST['usucpf'],$ultimomsgid);
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
	$sql_usuario = "select distinct u.usucpf as codigo, u.usunome as descricao 
from seguranca.usuario u inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.suscod='A' and us.sisid=".$_SESSION['sisid']." inner join monitora.projeto_ator pa on u.usucpf=pa.usucpf and pa.peastatus='A' and pa.pjeid=$id where u.suscod='A' order by descricao";	
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
<input type=hidden name="datavencida" value="0">
<input type=hidden name="id2" value="<?=$agrid?>">
<br>
 <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Evento da Agenda</td>
	 </tr>
	 <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('agrassunto','S','S','',70,100,'','');?></td>
     </tr>
	 <tr>
        <td align="right" class="subtitulodireita">Local:</td> 
        <td><?=campo_texto('agrlocal','S','S','',70,100,'','');?></td>
     </tr>
	 <tr>
        <td align="right" class="subtitulodireita">Data:</td> 
        <td><?=campo_data('agrdata', 'S','S','','S');?></td>
     </tr>     
     <tr>
        <td align="right" class="subtitulodireita">Hora:</td> 
        <td><?=campo_texto('agrhora', 'S','S','Entre com a hora hh:mm',5,5,"##:##",'');?></td>
     </tr> 
     <tr>
        <td align="right" class="subtitulodireita">Previsão de duração(min):</td> 
        <td><?=campo_texto('agrduracao', 'S','S','Entre com a previsão de duração em minutos',5,5,"###",'');?></td>
     </tr> 
   
	<tr>
        <td align='right' class="SubTituloDireita">Participantes:</td>
        <td>
        	<?
        	$sql_auxiliar = $_REQUEST[ 'agrid' ] ? "select distinct u.usucpf as codigo, u.usunome as descricao from agenda_reuniao_participante arp inner join seguranca.usuario u on arp.usucpf=u.usucpf  inner join agenda_reuniao ar on ar.agrid=arp.agrid inner join agenda_reuniao_processo ap on ap.agrid=ar.agrid and ap.$campo=$id where arp.usucpf=u.usucpf and arp.agrid=" . $_REQUEST[ 'agrid' ] . " order by descricao" : ''; 
        	$usucpf = $_REQUEST[ 'agrid' ] ? $db->carregar( $sql_auxiliar ) : ''; 
       		combo_popup( 'usucpf', $sql_usuario, 'Selecione o(s) Participante(s)', '360x460' )?>          </td>      
      </tr>                
     
     <tr align="center">
        <td colspan="2"><b>PAUTA</td> 
     </tr> 
     <?
     if ($_REQUEST['agrid'])
     {
     	// permitir incluir a ata da reunião
     	?>
     		<tr>
		<td class="SubTituloDireita">Arquivos vinculados à reunião:</td>
		<td>
			<? 	
				$sql=sql_vincula_arquivo('agenda_reuniao',$_REQUEST['agrid'],'agrid');
				popup_arquivo( 'arquivo_reuniao', $sql, 'agrid',$_REQUEST['agrid'], 1, 400, 400,'agenda_reuniao' );		
					
			?>
		</td>
	</tr>
     	<?
     }
     ?>
         
     <tr> 
        <td colspan=2><?=campo_textarea('agrpauta','N','S','','95%',10,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='<?=$botao?>' onclick="<?=$acao?>">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 
<script>

   function popup_arquivo( acao, nome, width, height )
	{
		window.open( '../geral/popup_arquivo.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	}
	
  function fechar_janela()
  {
    window.close();

  }
    function cria_registro()
  {
    prepara_formulario();

   	if (! validaBranco(document.formulario.agrassunto, 'Assunto')) return;
  	if (! validaBranco(document.formulario.agrlocal, 'Local')) return; 	
  	if (! validaBranco(document.formulario.agrdata, 'Data')) return;
 	if (! validaDataMaiorQueHoje(document.formulario.agrdata))
	{
	  if(! window.confirm( "Você agendou reunião com data vencida!\n Confirma sua escolha?\nNeste caso o sistema não irá enviar e-mail aos participantes.") )
		{
         	document.formulario.agrdata.focus();
			return;
		}
      	else {
      		document.formulario.datavencida.value=1;
      	}
	}	
  	if (!validaBranco(document.formulario.agrhora, 'Hora')) return; 
  	if (document.formulario.agrhora.value >'23:00')
		{
			alert("Hora inválida.");
			document.formulario.agrhora.focus();
			return;
		}   	
  	if (!validaBranco(document.formulario.agrduracao, 'Previsão de duração')) return;
  	
  	// verifica se selecionou algum usucpf selecionado
  	var selectUsucpf = document.getElementById( 'usucpf' );
  	var algumSelecionado = false;
	for ( var i = 0, j = selectUsucpf.options.length; i < j; i++ )
	{
		if ( selectUsucpf.options[i].value != '' )
		{
			algumSelecionado = true;
			break;
		}
	}
	if ( !algumSelecionado )
	{
		// ERRO!
		 alert("É necessário escolher os participantes da reunião!.");
		document.formulario.usucpf.focus();
		return;
	}
 	
  	
	if (!validaBranco(document.formulario.agrpauta, 'Pauta da reunião')) return tinyMCE.execCommand('mceFocus', true, 'agrpauta');
	document.formulario.act.value='incluir'	;
	document.formulario.submit();

  }
  
    function altera_registro(cod)
  {
    prepara_formulario();
   	if (!validaBranco(document.formulario.agrassunto, 'Assunto')) return;
  	if (!validaBranco(document.formulario.agrlocal, 'Local')) return; 	
  	if (!validaBranco(document.formulario.agrdata, 'Data')) return;
 	if (! validaDataMaiorQueHoje(document.formulario.agrdata))
	{
	  if(! window.confirm( "Você agendou reunião com data vencida!\n Confirma sua escolha?\nNeste caso o sistema não irá enviar e-mail aos participantes.") )
		{
         	document.formulario.agrdata.focus();
			return;
		}
      	else {
      		document.formulario.datavencida.value=1;
      	}
	}
  	if (!validaBranco(document.formulario.agrhora, 'Hora')) return; 
  	// verifica se selecionou algum usucpf selecionado
  	var selectUsucpf = document.getElementById( 'usucpf' );
  	var algumSelecionado = false;
	for ( var i = 0, j = selectUsucpf.options.length; i < j; i++ )
	{
		if ( selectUsucpf.options[i].value != '' )
		{
			algumSelecionado = true;
			break;
		}
	}
	if ( !algumSelecionado )
	{
		// ERRO!
		 alert("É necessário escolher os participantes da reunião!.");
		document.formulario.usucpf.focus();
		return;
	}  	
  	if (document.formulario.agrhora.value >'23:00')
		{
			alert("Hora inválida.");
			document.formulario.agrhora.focus();
			return;
		}  	
  	if (!validaBranco(document.formulario.agrduracao, 'Previsão de duração')) return;

	if (!validaBranco(document.formulario.agrpauta, 'Pauta da reunião')) return tinyMCE.execCommand('mceFocus', true, 'agrpauta');
	  	

	
	
	document.formulario.act.value='alterar'	;
	document.formulario.submit();

  }  

</script>
</body>
</html>
