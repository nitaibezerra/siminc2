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

function atualiza_destino_a($id, $usucpfs  )
	{
		global $db;
		$id = (integer) $id;
		if ( (!$id) or (! is_array($usucpfs)))
		{
			return;
		}
	
		$sql_remocao = "delete from comunicacao_destinatario where usucpf is not null and comid=".$id;
		$db->executar( $sql_remocao );

		foreach ( (array) $usucpfs as $usucpf )
		{
			if ($usucpf) {
				$sql_insercao = "insert into comunicacao_destinatario (comid, usucpf ) values ( " . $id . ",'" . $usucpf . "')";	
			$db->executar( $sql_insercao );

		}

		}

	} 

function atualiza_destino_b($id, $pflcods  )
	{
		global $db;
		$id = (integer) $id;
		if ( (!$id) or (! is_array($pflcods)))
		{
			return;
		}

		$sql_remocao = "delete from comunicacao_destinatario where pflcod is not null and comid=".$id;
		$db->executar( $sql_remocao );
		foreach ( (array) $pflcods as $pflcod )
		{
			if ($pflcod) {
				$sql_insercao = "insert into comunicacao_destinatario (comid, pflcod ) values ( " . $id . ",'" . $pflcod . "')";	
			$db->executar( $sql_insercao );
		}

		}

	} 	

$campo=$_REQUEST['campo'];
if ($campo=='pe') $campo='pjeid';
if ($_REQUEST['id']) $id=$_REQUEST['id'];
$botao='Criar Comunicação';
$acao="cria_registro()";

if ($_REQUEST['comid']) 
{
	// então está editando o registro
	$sql = "select c.*, cp.* from comunicacao c inner join comunicacao_processo cp on cp.comid=c.comid where c.comid=".$_REQUEST['comid'];
	$linha=$db->pegaLinha($sql);
	foreach($linha as $k=>$v) ${$k}=$v;
	$id=$copvalorcampo;
	$botao='Alterar Comunicação';
	$acao="altera_registro($comid)";
	

	
}



if ($_REQUEST['act']=='incluir')
{
	// inclusão de item de aviso
	  
        $sql = "insert into comunicacao (asscod,comdataini,comdatafim,comperiodicidade,usucpf) values (".
   "'".str_to_upper($_REQUEST['asscod']).
   "','".$_REQUEST['comdataini']."','".
   $_REQUEST['comdatafim']."',".
   $_REQUEST['comperiodicidade'].",".   
   "'".$_SESSION['usucpf']."')";
 
     $saida = $db->executar($sql);

     $sql =  "select comid from comunicacao where oid = ".pg_last_oid($saida);
     $comid = $db->pegaUm($sql);
       if (! $_REQUEST['pflcod'][0])   atualiza_destino_a($comid, $_REQUEST['usucpf'] );
       else atualiza_destino_b($comid, $_REQUEST['pflcod'] );
 
       // cria registro em comunicacao_processo
       $sql ="insert into comunicacao_processo (copcampo,copvalorcampo,comid) values ('pjeid',$id,$comid)";
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
	    $comid = $_REQUEST['id2'];
        $sql = "update comunicacao set asscod='".$_REQUEST['asscod']."',comperiodicidade=".$_REQUEST['comperiodicidade'].",usucpf='".$_SESSION['usucpf']."' ,comdataini='".$_REQUEST['comdataini']."',comdatafim='".$_REQUEST['comdatafim']."' where comid=$comid";
       $saida = $db->executar($sql);
	   if (! $_REQUEST['pflcod'][0])   atualiza_destino_a($comid, $_REQUEST['usucpf'] );
       else atualiza_destino_b($comid, $_REQUEST['pflcod'] );
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


$sql_usuario = "select distinct u.usucpf as codigo, u.usunome as descricao 
from seguranca.usuario u 
inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=".$_SESSION['sisid']." 
 where u.usustatus='A' order by descricao";


$sql_perfil = "select distinct p.pflcod as codigo, p.pfldsc as descricao 
from seguranca.perfil p  
where p.pflstatus='A' and p.sisid=".$_SESSION['sisid']." and p.pflnivel > 1 order by descricao";

if ($campo=='pjeid')
{
	$sql_usuario = "select distinct u.usucpf as codigo, u.usunome as descricao 
from seguranca.usuario u inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.suscod='A' and us.sisid=".$_SESSION['sisid']." inner join monitora.projeto_ator pa on u.usucpf=pa.usucpf and pa.peastatus='A' and pa.pjeid=$id where u.suscod='A' order by descricao";
	
}

?>
<html>
<head>
<title>Comunicação Programada</title>
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
<input type=hidden name="id2" value="<?=$comid?>">

<br>
 <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
	 <tr>
        <td align="right" class="subtitulodireita">Data inicial:</td> 
        <td><?=campo_data('comdataini', 'S','S','','S');?></td>
     </tr>
     <tr>
        <td align="right" class="subtitulodireita">Data final:</td> 
        <td><?=campo_data('comdatafim', 'S','S','','S');?></td>
     </tr> 
     <tr>
        <td align="right" class="subtitulodireita">Periodicidade (dias):</td> 
        <td><?=campo_texto('comperiodicidade', 'S','S','Entre com a periodicidade em dias',3,2,"##",'');?></td>
     </tr> 
 
	<tr>
        <td align='right' class="SubTituloDireita">Destinatário (pessoa):</td>
        <td>
        	<?
        	$sql_auxiliar = $_REQUEST[ 'comid' ] ? "select distinct u.usucpf as codigo, u.usunome as descricao from comunicacao_destinatario cd inner join seguranca.usuario u on cd.usucpf=u.usucpf  inner join comunicacao c on c.comid=cd.comid inner join comunicacao_processo cp on cp.comid=c.comid and cp.copvalorcampo=$id where cd.usucpf=u.usucpf and cd.comid=".$_REQUEST[ 'comid' ]." order by descricao" : ''; 
            $usucpf = $_REQUEST[ 'comid' ] ? $db->carregar( $sql_auxiliar ) : ''; 
       		combo_popup( 'usucpf', $sql_usuario, 'Selecione o(s) Participante(s)', '360x460' )?>          </td>      
      </tr>
	<tr>
        <td align='right' class="SubTituloDireita">Destinatário (Perfil):</td>
        <td>
        	<?
        	$sql_auxiliar = $_REQUEST[ 'comid' ] ? "select distinct p.pflcod as codigo, p.pfldsc as descricao from comunicacao_destinatario cd inner join seguranca.perfil p on cd.pflcod=p.pflcod  inner join comunicacao c on c.comid=cd.comid inner join comunicacao_processo cp on cp.comid=c.comid and cp.copvalorcampo=$id order by descricao" : ''; 
            $pflcod = $_REQUEST[ 'comid' ] ? $db->carregar( $sql_auxiliar ) : ''; 
       		combo_popup( 'pflcod', $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' )?>          </td>      
      </tr>      
     
        
     <tr> <td align='right' class="SubTituloDireita">Contexto:</td>
        <td ><? $sql = "select asscod as CODIGO,assdsc as DESCRICAO from assuntocomunicacao where assstatus='A' and sisid=".$_SESSION['sisid']." order by descricao ";
                  $db->monta_combo("asscod",$sql,$habil,"Escolha o contexto",'','','Escolha o contexto da comunicação.',400,'S');?></td>
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
    if (!validaBranco(document.formulario.comdataini, 'Data inicial')) return; 
    if (!validaBranco(document.formulario.comdatafim, 'Data final')) return;
if (!validaDataMaior(document.formulario.comdataini , document.formulario.comdatafim))
	{
		alert("Data Final não pode ser Anterior que Data Início.");
		document.formulario.comdatafim.focus();
		return;
	}    
  	if (!validaBranco(document.formulario.comperiodicidade, 'Periodicidade')) return;
 	if (!validaBranco(document.formulario.asscod, 'Contexto')) return;  	
	//verificação do campo corpo email

	document.formulario.act.value='incluir'	;
	document.formulario.submit();

  }
  function altera_registro(cod)
  {
    prepara_formulario();
    if (!validaBranco(document.formulario.comdataini, 'Data inicial')) return; 
    if (!validaBranco(document.formulario.comdatafim, 'Data final')) return;
if (!validaDataMaior(document.formulario.comdataini , document.formulario.comdatafim))
	{
		alert("Data Final não pode ser Anterior que Data Início.");
		document.formulario.comdatafim.focus();
		return;
	}     
  	if (!validaBranco(document.formulario.comperiodicidade, 'Periodicidade')) return;
 	if (!validaBranco(document.formulario.asscod, 'Contexto')) return; 

	document.formulario.act.value='alterar'	;
	document.formulario.submit();

  }  

</script>
</body>
</html>
