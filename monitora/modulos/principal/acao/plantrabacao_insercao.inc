<?
/*
 Sistema Simec
 Setor responsavel: SPO-MEC
 Desenvolvedor: Equipe Consultores Simec
 Analista: Gilberto Arruda Cerqueira Xavier
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Fabrício Mendonça(fabriciomendonca@gmail.com)
 Modificacoes: Thiago Mata thiago_mata@yahoo.com.br 9 de novembro de 2006
 Módulo:plantrabpje_insercao.inc
 Finalidade: este arquivo inclui os itens básicos de uma tarefa
 */
require_once( 'plantrabacao_funcoes.inc' );
$acaid=$_SESSION['acaid'];

if ($_REQUEST['act']=='inserir')
{	
	// achar o mais alto id para o código

	$ptoid=$_REQUEST['ptoid'];
	if (! $_REQUEST['ptoid'])
	{
		if ($_REQUEST['ptosnsubacao']) $ptosnsubacao='t' ; else $ptosnsubacao='f';
		$sql = "select * from monitora.planotrabalho where pjeid=7  limit 1";
		
		//TO DO ALTERAR O NOME DO CAMPO SUBACAO_PJE PARA SUBACAO
		
		$sql = "select ptoordemprov from monitora.planotrabalho where acaid=$acaid and ptonivel = 1 order by trim(ptocod)::integer desc limit 1";
		$maiorcod=$db->pegaum($sql);
		$novocod = $maiorcod+ 1;
        $sql = "insert into monitora.planotrabalho (ptosnsubacao,ptoprevistoexercicio,unmcod,ptotipo,acaid,ptodsc, ptocod, ptonivel, ptodescricao, ptoordemprov, usucpf, ptodata_ini, ptodata_fim) values ('$ptosnsubacao',100,2,'M',$acaid,'".$_REQUEST['ptodsc']."','".$novocod."',1,'".$_REQUEST['ptodescricao']."',$novocod,'".$_REQUEST['usucpf']."','".'01/01/'.$_SESSION['exercicio']."','".'31/12/'.$_SESSION['exercicio']."')";
        $saida=$db->executar($sql);
	}
	else 
	{
		// achar o nível e o código do meu pai
		$sql = "select ptonivel,ptocod from monitora.planotrabalho where ptoid=$ptoid"	;
		$dadospai=$db->pegalinha($sql);
		$meunivel = $dadospai['ptonivel']+1;

		$sql = "select ptoordemprov from monitora.planotrabalho where ptoid_pai=$ptoid and acaid=$acaid and ptonivel = $meunivel order by trim(ptocod)::integer desc limit 1";
	//dbg($sql);
		$maiorcod=$db->pegaum($sql);
	//dbg($maiorcod);
		$novocod = $maiorcod+ 1;
		$meucodigo =  trim($dadospai['ptocod']).$novocod;
    	$sql = "insert into monitora.planotrabalho (ptoprevistoexercicio,unmcod,ptoid_pai,ptotipo,acaid,ptodsc, ptocod, ptonivel, ptodescricao, ptoordemprov, usucpf, ptodata_ini, ptodata_fim) values (100,2,$ptoid,'M',$acaid,'".$_REQUEST['ptodsc']."','".$meucodigo."',$meunivel,'".$_REQUEST['ptodescricao']."',$novocod,'".$_REQUEST['usucpf']."','".$_REQUEST['dtini']."','".$_REQUEST['dtfim']."')";
        $saida=$db->executar($sql);
	
	}
	$sql =	' SELECT ' . 
				'ptoid' . 
			' FROM ' . 
				'monitora.planotrabalho' . 
			' WHERE '.
				'oid' . ' = ' . pg_last_oid($saida) .
			'';
	$ultimoPtoid = $db->pegaUm($sql);
	
	$sql =	'INSERT INTO ' .
				'monitora.plantrabacao ' .
				' ( ' . 
					'ptoid' . 
					',' . 
					'acaid' .
				' ) ' .
			' VALUES '.
				' ( ' .
					$ultimoPtoid . ',' . 
					$_SESSION[ 'acaid' ] . 
				' ) ' .
			'';
			
	$saida = $db->executar($sql);

		$db->commit();
		novo_ajusta_ordem();
        if ($saida)
        {
        ?>
          <script>
                alert ('Operação realizada com sucesso!');
                window.opener.location.reload();
        		self.close();
        		window.close();
          </script>
        <?
        $db->close();
    }
}

$ptodsc	=$_REQUEST['ptodsc'];
if ($_REQUEST['id']) {
	$ptoid=$_REQUEST['id'];
	$sql = "select ptodata_ini as ptodataini , ptodata_fim as ptodatafim from monitora.planotrabalho where ptoid=$ptoid";
	$datas = $db->pegaLinha($sql);
	$ptodataini= $datas['ptodataini'];
	$ptodatafim= $datas['ptodatafim'];	
}

//chamaCabecalho($ptoid);

?>

<link	rel="stylesheet" type="text/css" href="../includes/Estilo.css" >
<link	rel='stylesheet' type='text/css' href='../includes/listagem.css' >
<script language="JavaScript" src="../includes/funcoes.js"></script>

<br>
<?

$titulo_modulo = 'Estruturar o Plano de Trabalho';

$usucpf=$_SESSION['usucpf'];
monta_titulo($titulo_modulo,'');
?>
<div align="center">
	<center>
		<?
		// verifica se á coordenador de ação
		$autoriza = false;
		$coordacao=false;
		$digit=false;

		if ( $db->testa_coordenador( $_SESSION[ 'acaid' ],'A' ) ) $coordacao = true;
		// verifica se é digitador

		if ( $db->testa_digitador($_SESSION[ 'acaid' ],'A' ) ) $digit = true;
		
		// verific se á super-usuário
		if ( $db->testa_superuser() )
		{
			$coordacao = true;
			$_SESSION[ 'coordacao' ] = true;
			$digit = true;
		}

		// verifica se o exercício está aberto para estruturar o simec
		$sql= 	"select prsano 
				from monitora.programacaoexercicio 
				where prsano='".$_SESSION['exercicio']."' and prsstatus='A'";

		$registro=$db->recuperar($sql);

		if(	!$coordacao and !$digit and !$visivel )
		{
		?>
			<script>
       			alert ( 'Você não tem acesso para monitorar o Plano de Trabalho nesta Ação!');
			</script>
		<?
		exit();
		}
	/**
	 * Seleciona o somatório das previsões de desembolso do projeto especial em questão
	 */
	
	$sqlLimite =	" select " .
	    			" sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
				    " from monitora.planotrabalho p " .
				    " inner join monitora.plantrabacao pa on pa.ptoid=p.ptoid and pa.acaid = " . $_SESSION['acaid'] .
				    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
				    " where p.ptostatus='A' and p.ptoid_pai is null";

    if( $ptoid )
    {
    	$sqlLimite .= " and p.ptoid <> ".$ptoid;
    }
    $totalGasto = $db->pegaUm( $sqlLimite );
	    
    //calcula o valor que ainda pode ser utilizado nas atividades do projeto especial
    $limiteProjeto = $pjevlrano - $totalGasto;

    
?>
<form method="POST" name="formulario" id="formulario">
<input	type='hidden'	name="modulo"		value="<?=$modulo?>"	> 
<input	type='hidden'	name="ptoid"		value="<?=$ptoid?>"	>
<input type='hidden' name='dtini' value="<?=$ptodataini?>">
<input type='hidden' name='dtfim' value="<?=$ptodatafim?>">	
<input	type='hidden'	name='exclui'		value=0	>
<input	type='hidden'	name='act'	>
<div id="camposDinamicos" style="visibility:hidden;margin:0; padding:0;"></div>
<center>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <? if (! $ptoid) {?>
	<tr>
        <td align='right' class="SubTituloDireita"><b>SUBAÇÃO:</td>
		<td>
			<input type="checkbox" name="ptosnsubacao" title="Marque esta opção se desejar que se constitua em uma subação">
		</td>
    </tr>
    <?}?>
	<tr>
        <td align='right' class="SubTituloDireita">Título:</td>
		<td>
			<?=campo_texto('ptodsc','S','S','',77,80,'','','','Entre com o título (ou nome) que esta atividade será conhecida.');?>
		</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
        <td><?=campo_textarea('ptodescricao','S','S','',80,3,'','','',' Campo utilizado para descrever o motivo da criação da atividade.');?></td>
      </tr>  
      
      <tr>
        <td align='right' class="SubTituloDireita">Responsável:</td>
	    <td><? $sql = "select distinct u.usucpf as CODIGO,u.usucpf ||'-'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as DESCRICAO, u.usunome from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.acaid=$acaid where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod in (1,8)) and u.usustatus='A' order by usunome ";
	    $db->monta_combo("usucpf",$sql,'S',"Selecione o Responsável",'','','',400);
	    ?></td>
      </tr>  
      <tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="window.close();" class="botao"></td>

 </tr>
	</tbody>
</table>
</center>
</div>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
</form>
<script>
  function validar_cadastro()
  {
  	 
  	if (!validaBranco(document.formulario.ptodsc, 'Título')) return;
	if (!validaBranco(document.formulario.ptodescricao, 'Descrição')) return;		
	if (!validaBranco(document.formulario.usucpf, 'Responsável')) return;		
  	
  	document.formulario.act.value='inserir';
  	 
  	 document.formulario.submit();
  }
</script>