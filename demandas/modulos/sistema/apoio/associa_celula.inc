 <?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:associa_celula.inc
   Finalidade: permitir a associação de um celula as operações de celula
   */
   

$modulo=$_REQUEST['modulo'] ;

if ($_REQUEST['act']=='cadastra' and $_POST['celid']<>'') {
  
  $sidid = $_POST['sidid'];
  //print_r($_POST['sidid']);
  $nlinhas = count($sidid)-1;
	for ($j=0; $j<=$nlinhas;$j++)
	{
		  $sql = "insert into demandas.sistemacelula (celid, sidid) VALUES (".$_POST['celid'].",".$_POST['sidid'][$j].")";
		 // print $_POST['sidid'][$j].'<br>';
		  $db->executar($sql);
	 }
  $db -> commit();
  //$db->sucesso($modulo);
  
    ?>
    	<script>
            alert('Operação realizada com sucesso!');
		  	//location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A&celid=<?=$celid?>&op=update';
		  	window.opener.location.reload(); 
		  	self.close(); 
        </script>
    <?
	exit();  
  
}

$celid = $_REQUEST['celid'];

?>


<html>
<head>
<title><?php echo SIGLA_SISTEMA; ?> - Associar Sistemas</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>

<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<input type=hidden name="celid" value="<?=$celid?>">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr><td ALIGN=center>
			<B>Inserir sistemas para:</B>
			<?
				  $sql = "select 
				  			--c.celid as CODIGO, 
				  			c.celnome || ' - Gerente: ' || 
							CASE 
								WHEN u.usunome != '' THEN u.usunome
								ELSE ' - '
							END as DESCRICAO  
							from demandas.celula c
				 			LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
				 			LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
				  		  where 
				  		  	c.celstatus='A'
				  		  	and
				  		  	C.CELID = ".$celid." 
				  		  order by c.celnome ";
				  echo $db->PegaUm($sql);
				?>	
		
		

		</td>
	</tr>
	</table>
<?

//teste utilizando a função Monta Lista
$cabecalho = array('Sim/Não','Código','<div align=left>Nome</div>');
//$sql = "select acacod, acadsc from acao";
$sql= "select  
			case when c.sidid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' else '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>' end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid  
		where  s.sidstatus = 'A' 
		and c.celid IS NULL
		order by 3";
//print $sql;
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr  bgcolor="#CCCCCC"><td><input type='button' value='Inserir' onclick="validar_cadastro()"></td></tr>
</table>

</form>

</body>
</html>

<script language="JavaScript">

  function validar_cadastro() {

    document.formulario.act.value = 'cadastra';
	document.formulario.submit();
  }


</script>