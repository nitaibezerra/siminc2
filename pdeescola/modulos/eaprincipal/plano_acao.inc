<?php

ini_set( "memory_limit", "512M" );
set_time_limit(0);
/*** Verifica se as variáveis de sessão do módulo estão corretamente setadas ***/
eaVerificaSessao();

/*** Verifica se há algum tipo de parceria cadastrada ***/
/*$sql = "SELECT
			eapossuiparceria
	    FROM
	   		pdeescola.eacescolaacessivel
	   	WHERE
	   		eacid = {$_SESSION['eacid']}";
$dados = $db->pegaUm($sql);
if(!$dados)
{
	echo "<script>
			alert('Uma das opções deve ser escolhida para o Plano de Atendimento ficar disponível.');
			location.href = 'pdeescola.php?modulo=eaprincipal/diagnostico&acao=A';
		  </script>";
	die;
}*/

include_once APPRAIZ . 'includes/workflow.php';

// AJAX
if($_POST['ajaxEtaid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
				etiid AS codigo, 
				etidescricao AS descricao 
			FROM
				pdeescola.eactipoitemfinanciavel 
			WHERE
				etaid = {$_POST['ajaxEtaid']}
				AND etistatus = 'A'
				AND etianoreferencia = {$_SESSION['exercicio']}
			ORDER BY
				etidescricao ASC";
	die($db->monta_combo("etiid", $sql, 'S', 'Selecione um ou mais itens financiáveis, sequencialmente...', 'verificaTipoItens', '', '', '700', 'S', ''));
}

if( $_POST['ajaxTipoItens'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
				etdid
			FROM
				pdeescola.eactipoitemfinanciavel
			WHERE
				etiid = {$_POST['ajaxTipoItens']}
				AND etistatus = 'A'
				AND etianoreferencia = {$_SESSION['exercicio']}
			ORDER BY
				etidescricao ASC";
				
	$etdid = $db->pegaUm( $sql );
	echo $etdid;
	exit();
}

// função para recuperar os limites de gasto de custeio/capital
function recuperaLimites($inicio, $fim, $parceria)
{
	$retorno = array();
	
	if( (integer)$_SESSION['exercicio'] == 2014 )
    {
       if(($inicio == 1 && $fim == 199)) {
           $recurso = 8300;
       }
       if(($inicio == 200 && $fim == 499)) {
           $recurso = 10000;
       }
       if(($inicio == 500 && $fim == 1000)) {
           $recurso = 12500;
       }     
       if(($inicio == 0) && ($fim == 9999)) {
           $recurso = 15000;
       }
       if(($inicio == 1000 || $inicio == 1001) && ($fim == 10000)) {
           $recurso = 15000;
       }

		$retorno["custeio"] = $recurso * 0.8;
		$retorno["capital"] = $recurso * 0.2;
    }
    else if( (integer)$_SESSION['exercicio'] == 2013 )
    {
       if(($inicio == 1 && $fim == 199)) {
           $recurso = 8300;
       }
       if(($inicio == 200 && $fim == 499)) {
           $recurso = 10000;
       }
       if(($inicio == 500 && $fim == 1000)) {
           $recurso = 12500;
       }     
       if(($inicio == 0) && ($fim == 9999)) {
           $recurso = 15000;
       }

		$retorno["custeio"] = $recurso * 0.8;
		$retorno["capital"] = $recurso * 0.2;
    }
    else if( (integer)$_SESSION['exercicio'] == 2012 )
	{
		if(($inicio >= 0 && $inicio <= 199) && ($fim >= 0 && $fim <= 199)) {
			$recurso = 8300;
		}
		if(($inicio >= 200 && $inicio <= 499) && ($fim >= 200 && $fim <= 499)) {
			$recurso = 10000;
		}
		if(($inicio >= 500 && $inicio <= 1000) && ($fim >= 500 && $fim <= 1000)) {
			$recurso = 12500;
		}	
		if(($inicio > 1000) && ($fim >1000)) {
			$recurso = 15000;
		}
		
		$retorno["custeio"] = $recurso * 0.8;
		$retorno["capital"] = $recurso * 0.2;
	} else if( (integer)$_SESSION['exercicio'] == 2011 )
	{
		if(($inicio >= 0 && $inicio <= 199) && ($fim >= 0 && $fim <= 199)) {
			$recurso = 6000;
		}
		if(($inicio >= 200 && $inicio <= 499) && ($fim >= 200 && $fim <= 499)) {
			$recurso = 7000;
		}
		if(($inicio >= 500 && $inicio <= 1000) && ($fim >= 500 && $fim <= 1000)) {
			$recurso = 8000;
		}	
		if(($inicio > 1000) && ($fim >1000)) {
			$recurso = 9000;
		}
		
		$retorno["custeio"] = $recurso * 0.8;
		$retorno["capital"] = $recurso * 0.2;
	}
	else
	{
		if(($inicio >= 0 && $inicio <= 199) && ($fim >= 0 && $fim <= 199)) {
			$recurso = ($parceria) ? 15600 : 12000;
		}
		if(($inicio >= 200 && $inicio <= 499) && ($fim >= 200 && $fim <= 499)) {
			$recurso = ($parceria) ? 18200 : 14000;
		}
		if(($inicio >= 500 && $inicio <= 1000) && ($fim >= 500 && $fim <= 1000)) {
			$recurso = ($parceria) ? 20800 : 16000;
		}	
		if(($inicio > 1000) && ($fim >1000)) {
			$recurso = ($parceria) ? 23400 : 18000;
		}
		
		$retorno["custeio"] = $recurso * 0.6;
		$retorno["capital"] = $recurso * 0.4;
	}
	
	return $retorno;
}

// Formulário
if($_REQUEST["submetido"])
{
	if(!$_REQUEST["excluir"] || $_REQUEST["excluir"] == "")
	{
		/*** Verifica se é possível inserir valor, ou se ultrapassou os limites de capital/custeio ***/
		
		$intervalo 			= $db->carregar("SELECT eaciniciointervalo, eacfimintervalo FROM pdeescola.eacescolaacessivel WHERE eacid = ".$_SESSION['eacid']);
		$inicioIntervalo = (integer)$intervalo[0]["eaciniciointervalo"];
		$fimIntervalo 	= (integer)$intervalo[0]["eacfimintervalo"];
		
		// recupera a parceria
		$parceria = $db->carregar("SELECT entid FROM pdeescola.eacentidade WHERE eacid = {$_SESSION['eacid']}");
		$parceria = ($parceria) ? true : false;
		// Recupera os limites
		$limites = recuperaLimites($inicioIntervalo, $fimIntervalo, $parceria);
		
		// Recupera os valores
		$somacusteio = 0;
		$somacapital = 0;
		// calcula o total gasto com custeio e capital
		$sql = "SELECT
					CASE WHEN eaa.eaavlrcusteio is null
					THEN 0.00 ELSE (eaa.eaavlrcusteio * eaa.eaaqtd) END as somacusteio,
					CASE WHEN eaa.eaavlrcapital is null
					THEN 0.00 ELSE (eaa.eaavlrcapital * eaa.eaaqtd) END as somacapital
				FROM
					pdeescola.eacacaoacessibilidade eaa
				INNER JOIN
					pdeescola.eactipoitemfinanciavel eti ON eti.etiid = eaa.etiid
														AND eti.etistatus = 'A'
														AND eti.etianoreferencia = {$_SESSION["exercicio"]} 
				INNER JOIN
					pdeescola.eactipoacaoacessibilidade eta ON eta.etaid = eti.etaid
														   AND eta.etaastatus = 'A'
														   AND eta.etaanoreferencia = {$_SESSION["exercicio"]}
				WHERE
					eaa.eacid = {$_SESSION["eacid"]}";
		$somaCusteioCapital = $db->carregar($sql);
		
		if($somaCusteioCapital)
		{
			for($i=0; $i<count($somaCusteioCapital); $i++)
			{
				$somacusteio += (float)$somaCusteioCapital[$i]["somacusteio"];
				$somacapital += (float)$somaCusteioCapital[$i]["somacapital"];
			}
		}
		
		/*$dados = $db->carregar("SELECT
									eac.eaciniciointervalo as inicio_intervalo,
									eac.eacfimintervalo as fim_intervalo,
									coalesce(sum(eaa.eaavlrcusteio * eaa.eaaqtd), 0) as custeio,
									coalesce(sum(eaa.eaavlrcapital * eaa.eaaqtd), 0) as capital,
									eace.entid as parceria
								FROM 
									pdeescola.eacescolaacessivel eac
								INNER JOIN
									pdeescola.eacacaoacessibilidade eaa ON eaa.eacid = eac.eacid
								LEFT JOIN
									pdeescola.eacentidade eace ON eace.eacid = eac.eacid
								WHERE
									eac.eacid = ".$_SESSION["eacid"]."
								GROUP BY
									inicio_intervalo,
									fim_intervalo,
									parceria");
		$inicioIntervalo 	= $dados[0]['inicio_intervalo'];
		$fimIntervalo 		= $dados[0]['fim_intervalo'];
		$custeio 			= $dados[0]['custeio'];
		$capital 			= $dados[0]['capital'];
		$parceria 			= $dados[0]['parceria'];
		
		
		
		dbg($custeio);
		dbg($capital);*/
		
		$totalCusteio = ($somacusteio + $_REQUEST["eaavlrcusteio"]);
		$totalCapital = ($somacapital + $_REQUEST["eaavlrcapital"]);
		
		/*** Se o valor inserido de custeio for maior que o limite ***/
		if( bccomp($totalCusteio, $limites["custeio"], 2) === 1 )
		{
			echo '<script>
					alert("O valor inserido de custeio não pode exceder o limite permitido.");
					window.location.href = "pdeescola.php?modulo=eaprincipal/plano_acao&acao=A";
				  </script>';
			die;
		}
		/*** Se o valor inserido de capital for maior que o limite ***/
		elseif( bccomp($totalCapital, $limites["capital"], 2) === 1 )
		{
			echo '<script>
					alert("O valor inserido de capital não pode exceder o limite permitido.");
					window.location.href = "pdeescola.php?modulo=eaprincipal/plano_acao&acao=A";
				  </script>';
			die;
		}
		/*** Se os valores estão OK... ***/
		else
		{
			// Tratando os valores em reais para insert no banco...
			$_REQUEST["eaavlrcusteio"] = str_replace(",",".",str_replace(".","",$_REQUEST["eaavlrcusteio"]));
			$_REQUEST["eaavlrcapital"] = str_replace(",",".",str_replace(".","",$_REQUEST["eaavlrcapital"]));
				
			$_REQUEST["eaavlrcusteio"] = ($_REQUEST["eaavlrcusteio"]) ? $_REQUEST["eaavlrcusteio"] : 'NULL';
			$_REQUEST["eaavlrcapital"] = ($_REQUEST["eaavlrcapital"]) ? $_REQUEST["eaavlrcapital"] : 'NULL';
			
			// insert
			if(!$_REQUEST["eaaid"] || $_REQUEST["eaaid"] == "") {
				$sql = "INSERT INTO
							pdeescola.eacacaoacessibilidade(eacid, etiid, eaadescricao, eaaqtd, eaavlrcusteio, eaavlrcapital)
						VALUES
							({$_SESSION["eacid"]}, 
							 {$_REQUEST["etiid"]}, 
							 '".pg_escape_string($_REQUEST["eaadescricao"])."', 
							 {$_REQUEST["eaaqtd"]},
							 {$_REQUEST["eaavlrcusteio"]},
							 {$_REQUEST["eaavlrcapital"]})";
				$db->executar($sql);
			}
			// update
			else {
				$sql = "UPDATE
							pdeescola.eacacaoacessibilidade
						SET
							eacid = {$_SESSION["eacid"]}, 
							etiid = {$_REQUEST["etiid"]}, 
							eaadescricao = '".pg_escape_string($_REQUEST["eaadescricao"])."', 
							eaaqtd = {$_REQUEST["eaaqtd"]}, 
							eaavlrcusteio = {$_REQUEST["eaavlrcusteio"]}, 
							eaavlrcapital = {$_REQUEST["eaavlrcapital"]}
						WHERE
							eaaid = {$_REQUEST["eaaid"]}";
				$db->executar($sql);
				
				unset($_REQUEST["eaaid"]);
			}
		}
	}
	// delete 
	else
	{
		$sql = "DELETE FROM
					pdeescola.eacacaoacessibilidade
				WHERE
					eaaid = {$_REQUEST["excluir"]}";
		$db->executar($sql);
	}
	
	$db->commit();
	$db->sucesso('eaprincipal/plano_acao');
}

// Recupera os valores
if($_REQUEST["eaaid"]) {
	$sql = "SELECT
				eta.etaid,
				eti.etiid,
				eaa.eaadescricao,
				eaa.eaaqtd,
				eaa.eaavlrcusteio,
				eaa.eaavlrcapital,
				CASE WHEN eaa.eaavlrcusteio is null
				THEN (eaa.eaavlrcapital * eaa.eaaqtd)
				ELSE (eaa.eaavlrcusteio * eaa.eaaqtd)
				END AS totalrecurso
			FROM
				pdeescola.eacacaoacessibilidade eaa
			INNER JOIN
				pdeescola.eactipoitemfinanciavel eti ON eti.etiid = eaa.etiid 
			INNER JOIN
				pdeescola.eactipoacaoacessibilidade eta ON eta.etaid = eti.etaid 
			WHERE
				eaaid = {$_REQUEST["eaaid"]}";
	$dadosPlanoAcao = $db->carregar($sql);
	
	// extrai os dados
	if($dadosPlanoAcao[0]) extract($dadosPlanoAcao[0]);
	// trata os dados para exibição
	$eaavlrcusteio = number_format($eaavlrcusteio, 2, ',', '.');
	$eaavlrcapital = number_format($eaavlrcapital, 2, ',', '.');
	$totalrecurso  = number_format($totalrecurso, 2, ',', '.');
	// seta o eaaid para realizar o UPDATE
	$eaaid = $_REQUEST["eaaid"];
}

include APPRAIZ . "includes/cabecalho.inc";

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$sql = "SELECT docid FROM pdeescola.eacescolaacessivel WHERE eacid = ".$_SESSION["eacid"]." AND eacstatus = 'A'";
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = eaCriarDocumento( $_SESSION['entid'] , $_SESSION['eacid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Escola Acessível');
				location.href = 'pdeescola.php?modulo=inicio&acao=C';
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadEscolaAcessivel = $db->pegaUm($sql);

if(( integer)$vPerfilCadEscolaAcessivel > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == eaMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid'] );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Escola Acessível');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = eaPegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_EA || (integer)$esdid == CORRECAO_CADASTRAMENTO_EA) {
		$somenteConsulta = false;
	}
}

/********** TESTE PARA ESCOLAS QUE ESTÃO A RECEBER MAIS DO QUE DEVEM ***************/
/*$sql = "SELECT DISTINCT eacid FROM pdeescola.eacacaoacessibilidade";
$eacids = $db->carregarColuna($sql);

$contador=0;
$contador2=0;
foreach($eacids as $key=>$value)
{
	$intervalo 			= $db->carregar("SELECT eaciniciointervalo, eacfimintervalo FROM pdeescola.eacescolaacessivel WHERE eacid = ".$value);
	$eaciniciointervalo = (integer)$intervalo[0]["eaciniciointervalo"];
	$eacfimintervalo 	= (integer)$intervalo[0]["eacfimintervalo"];
	
	$total = $db->pegaUm('SELECT coalesce(sum(eaavlrcusteio * eaaqtd),0) + coalesce(sum(eaa.eaavlrcapital * eaa.eaaqtd),0) as total
						  FROM pdeescola.eacacaoacessibilidade eaa
						  WHERE eacid='.$value);
	
	$parceria = $db->carregar("SELECT entid FROM pdeescola.eacentidade WHERE eacid = ".$value);
	
	$limites = recuperaLimites($eaciniciointervalo, $eacfimintervalo, $parceria);
	
	$entcodent = $db->pegaUm('SELECT entcodent FROM pdeescola.eacescolaacessivel WHERE eacid='.$value);
	
	if( $total > ($limites["custeio"] +  $limites["capital"]) )
	{
		$contador++;
		echo 'CuLLLLpada -> '.$entcodent.'<br />';
	}
	else
	{
		$contador2++;
	}
}
echo '<br />total c/ erros: ' . $contador.'<br />total s/ erros: ' . $contador2;*/
/********************************************************************/

// recupera o início/fim do intervalo de alunos
$intervalo 			= $db->carregar("SELECT eaciniciointervalo, eacfimintervalo FROM pdeescola.eacescolaacessivel WHERE eacid = ".$_SESSION['eacid']);
$eaciniciointervalo = (integer)$intervalo[0]["eaciniciointervalo"];
$eacfimintervalo 	= (integer)$intervalo[0]["eacfimintervalo"];

// recupera a parceria
$parceria = $db->carregar("SELECT entid FROM pdeescola.eacentidade WHERE eacid = {$_SESSION['eacid']}");
$parceria = ($parceria) ? true : false;

// recupera o limite para custeio e capital
$limites = recuperaLimites($eaciniciointervalo, $eacfimintervalo, $parceria);

$somacusteio = 0;
$somacapital = 0;
// calcula o total gasto com custeio e capital
$sql = "SELECT
			CASE WHEN eaa.eaavlrcusteio is null
			THEN 0.00 ELSE (eaa.eaavlrcusteio * eaa.eaaqtd) END as somacusteio,
			CASE WHEN eaa.eaavlrcapital is null
			THEN 0.00 ELSE (eaa.eaavlrcapital * eaa.eaaqtd) END as somacapital
		FROM
			pdeescola.eacacaoacessibilidade eaa
		INNER JOIN
			pdeescola.eactipoitemfinanciavel eti ON eti.etiid = eaa.etiid
												AND eti.etistatus = 'A'
												AND eti.etianoreferencia = {$_SESSION["exercicio"]} 
		INNER JOIN
			pdeescola.eactipoacaoacessibilidade eta ON eta.etaid = eti.etaid
												   AND eta.etaastatus = 'A'
												   AND eta.etaanoreferencia = {$_SESSION["exercicio"]}
		WHERE
			eaa.eacid = {$_SESSION["eacid"]}";
$somaCusteioCapital = $db->carregar($sql);

if($somaCusteioCapital)
{
	for($i=0; $i<count($somaCusteioCapital); $i++)
	{
		$somacusteio += (float)$somaCusteioCapital[$i]["somacusteio"];
		$somacapital += (float)$somaCusteioCapital[$i]["somacapital"];
	}
}

// Calcula o valor restante de custeio/capital para cadastro das ações
$restanteCusteio = ($dadosPlanoAcao[0]["eaavlrcusteio"]) ? (( (float) bcsub($limites["custeio"], $somacusteio, 2) ) + ((float)$dadosPlanoAcao[0]["eaavlrcusteio"] * (integer)$dadosPlanoAcao[0]["eaaqtd"])) : ( (float) bcsub($limites["custeio"], $somacusteio, 2) );
$restanteCapital = ($dadosPlanoAcao[0]["eaavlrcapital"]) ? (( (float) bcsub($limites["capital"], $somacapital, 2) ) + ((float)$dadosPlanoAcao[0]["eaavlrcapital"] * (integer)$dadosPlanoAcao[0]["eaaqtd"])) : ( (float) bcsub($limites["capital"], $somacapital, 2) );

echo "<br />";
echo montarAbasArray(carregaAbasEscolaAcessivel(), "/pdeescola/pdeescola.php?modulo=eaprincipal/plano_acao&acao=A");

$titulo = 'Escola Acessível';
$subtitulo = '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório';

monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAcessivel();

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/eaajax.js" type="text/javascript"></script>

<form method="post" id="formPlanoAcao" name="formPlanoAcao" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="eaaid" value="<?=$eaaid?>" />
<input type="hidden" name="excluir" value="" />
<input type="hidden" name="ano" id="ano" value="<?=$_SESSION['exercicio'] ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="15" cellPadding="0" align="center" border="0">
	<tr>
		<td colspan="3">
			Ação
			<br />
			<? 
			$sql = "SELECT
						etaid as codigo,
						etadescricao as descricao
					FROM
						pdeescola.eactipoacaoacessibilidade
					WHERE
						etaastatus = 'A'
						AND etaanoreferencia = {$_SESSION['exercicio']}
					ORDER BY
						etadescricao ASC";
			$db->monta_combo("etaid", $sql, 'S', "Selecione uma ou mais ações, sequencialmente...", 'filtraItemFinanciavel', '', '', '700', 'S', '');
			?>
		</td>
		<td rowspan="3">
		<?
		$perfis = arrayPerfil();
		
		if($docid) {
			if( !in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $perfis) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
			{
				// Barra de estado atual e ações e Historico
				wf_desenhaBarraNavegacao( $docid , array( 'eacid' => $_SESSION['eacid'] ) );
			}
		}
		?>
		</td>
	</tr>
	<tr>
		<td id="item_financiavel" colspan="3">
			Itens Financiáveis
			<br />
			<select name="etiid" class="campoEstilo" style="width:700px;">
				<option value="">Selecione um ou mais itens financiáveis, sequencialmente...</option>
			</select>
			<img src="../imagens/obrig.gif" border="0" />
		</td>
	</tr>
	<tr>
		<td width="40%">
			Descrição do item
			<br />
			<?=campo_textarea( 'eaadescricao', 'S', 'S', '', '100', '5', '1000', '', 0, '', false, NULL, NULL)?>
		</td>
		<td align="center" align="top" width="60%">
			<table border="0" cellSpacing="10">
				<tr>
					<td align="right">Quantidade (un)</td>
					<td><?=campo_texto('eaaqtd', 'S', 'S', '', '25', '5', '#####', 'N', 'left', '', '', '', 'calculaTotalRecurso();', null, "this.value=mascaraglobal('#####',this.value);" )?></td>
				</tr>
				<tr>
					<td align="right">Valor unitário de Custeio (R$)</td>
					<td>
						<?=campo_texto('eaavlrcusteio', 'S', 'S', '', '25', '25', '[###.]###,##', 'N', 'left', '', 0, '', 'calculaTotalRecurso();', null, '' )?>
						<br />
						<span style="color:red;">Restante custeio: R$ <span id="restante_custeio"><?=number_format($restanteCusteio, 2, ",", ".")?></span></span>
					</td>
				</tr>
				<tr>
					<td align="right">Valor unitário de Capital (R$)</td>
					<td>
						<?=campo_texto('eaavlrcapital', 'S', 'S', '', '25', '25', '[###.]###,##', 'N', 'left', '', 0, '', 'calculaTotalRecurso();', null, '' )?>
						<br />
						<span style="color:red;">Restante capital: R$ <span id="restante_capital"><?=number_format($restanteCapital, 2, ",", ".")?></span></span>
					</td>
				</tr>
				<?
				/*** Desabilita a opção de salvar, se necessário ***/
				if( !in_array(PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL, $perfis) && !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
				{
					$acao = '<center><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\">
					         <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\"></center>';
					$btSalvar = "document.getElementById('btSalvarPlanoAcao').disabled = 1;";
				}
				else
				{
					$acao = '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarPlanoAcao(\' || eaa.eaaid || \');\">
					         <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirPlanoAcao(\' || eaa.eaaid || \')\"></center>';
					$btSalvar = "";
				}
				?>
				<tr>
					<td align="center" colspan="2">
						<input type="button" value="Salvar" id="btSalvarPlanoAcao" onclick="salvarPlanoAcao();" />
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#c0c0c0" cellSpacing="5" cellPadding="0" align="center" border="0">
	<tr>
		<td align="center">
			<b>Ações promoverão a acessibilidade ao(s) aluno(s) público alvo da educação especial</b>
		</td>
	</tr>
</table>
</form>

<?

$sql = "SELECT
			'".$acao."' as acoes,
			eta.etadescricao as acao,
			eti.etidescricao as item,
			eaa.eaadescricao as descricao,
			eaa.eaaqtd || ' ' as quantidade,
			
			CASE WHEN eaa.eaavlrcusteio is null
				THEN 0.00 || ' '
				ELSE eaa.eaavlrcusteio || ' '
			END AS custeio,
		
			CASE WHEN eaa.eaavlrcusteio is null
				THEN 0.00 * eaa.eaaqtd
				ELSE eaa.eaavlrcusteio * eaa.eaaqtd
			END AS total_custeio,
			
			CASE WHEN eaa.eaavlrcapital is null
				THEN 0.00 || ' '
				ELSE eaa.eaavlrcapital || ' '
			END AS capital,
			
			CASE WHEN eaa.eaavlrcapital is null
				THEN 0.00 * eaa.eaaqtd
				ELSE eaa.eaavlrcapital * eaa.eaaqtd
			END AS total_capital,
			
			CASE WHEN eaavlrcusteio is null
			THEN (eaavlrcapital * eaaqtd) 
			ELSE (eaavlrcusteio * eaaqtd)
			END AS total
		FROM
			pdeescola.eacacaoacessibilidade eaa
		INNER JOIN
			pdeescola.eactipoitemfinanciavel eti ON eti.etiid = eaa.etiid
												AND eti.etistatus = 'A'
												AND eti.etianoreferencia = {$_SESSION["exercicio"]} 
		INNER JOIN
			pdeescola.eactipoacaoacessibilidade eta ON eta.etaid = eti.etaid
												   AND eta.etaastatus = 'A'
												   AND eta.etaanoreferencia = {$_SESSION["exercicio"]}
		WHERE
			eaa.eacid = {$_SESSION["eacid"]}";

//dbg($sql);
$cabecalho = array("Nº", "Alterar/Excluir", "Ação", "Item", "Descrição", "Quantidade", "Valor unitário de Custeio","Valor Total de Custeio","Valor unitário de Capital","Valor Total do Capital","Total de Recurso");
//$db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '95%', 'S', false);

$tamanho		= array('1%', '5%', '26%','10%','15%','5%','8%','7%','8%','7%','8%');
$alinhamento	= array('center', 'center', 'left', 'center', 'center', 'right', 'right', 'right', 'right','right','right');

$arrListaDados = $db->carregar($sql);
$cont = 1;
$arrDados = array();

if( $arrListaDados )
{
	foreach($arrListaDados as $chave=>$resultado){
		$arrDados[$chave]['numero'] = $cont;
		foreach($resultado as $k =>$dado ){
			$arrDados[$chave][$k] = $dado;
		}
		$cont++;
	}
}

$db->monta_lista($arrDados, $cabecalho, 9000, 10, 'S','center', 'S', '', $tamanho, $alinhamento);
//$db->monta_lista_array($arrDados, $cabecalho, 9000, 10, 'S', 'center', '','','', $tamanho, $alinhamento );
?>

<script type="text/javascript">

<?=$btSalvar?>

var valorRestanteCusteio = <?=$restanteCusteio?>;
var valorRestanteCapital = <?=$restanteCapital?>;

function salvarPlanoAcao() {
	var form	=	document.getElementById("formPlanoAcao");
	var bt		=	document.getElementById("btSalvarPlanoAcao");
	var acao	=	document.getElementsByName('etaid')[0];
	var item	=	document.getElementsByName('etiid')[0];
	var desc	=	document.getElementsByName('eaadescricao')[0];
	var qtd		=	document.getElementsByName('eaaqtd')[0];
	var custeio	=	document.getElementsByName('eaavlrcusteio')[0];
	var capital	=	document.getElementsByName('eaavlrcapital')[0];
	
	bt.disabled 	= true;
	
	if(acao.value == "") {
		alert('O campo "Ação" deve ser preenchido.');
		acao.focus();
		bt.disabled = false;
		return;
	}
	if(item.value == "") {
		alert('O campo "Itens Financiáveis" deve ser preenchido.');
		item.focus();
		bt.disabled = false;
		return;
	}
	if(desc.value == "") {
		alert('O campo "Descrição do item" deve ser preenchido.');
		desc.focus();
		bt.disabled = false;
		return;
	}
	if(qtd.value == "") {
		alert('O campo "Quantidade (un)" deve ser preenchido.');
		qtd.focus();
		bt.disabled = false;
		return;
	}
	if(Number(qtd.value) <= 0) {
		alert('O campo "Quantidade (un)" deve ser maior que zero (0).');
		qtd.focus();
		bt.disabled = false;
		return;
	}
	if(capital.value == "" && custeio.value == "") {
		alert('Pelo menos um campo de Valor (Custeio ou Capital) deve ser preenchido.');
		custeio.focus();
		bt.disabled = false;
		return;
	}
	if(capital.value != "" && custeio.value != "") {
		alert('Deve ser preenchido somente um campo de Valor (Custeio ou Capital).');
		custeio.focus();
		bt.disabled = false;
		return;
	}
	if( (Number(converteValorString(custeio.value)) * qtd.value) > Number(valorRestanteCusteio) ) {
		alert('O valor de Custeio não deve execeder o valor do "Restante custeio".');
		custeio.focus();
		bt.disabled = false;
		return;
	}
	if( (Number(converteValorString(capital.value)) * qtd.value) > Number(valorRestanteCapital) ) {
		alert('O valor de Capital não deve execeder o valor do "Restante capital".');
		custeio.focus();
		bt.disabled = false;
		return;
	}
	
	form.submit();
}

function converteValorString(valor) {
	valor = valor.toString().replace(/\./g, "");
	valor = valor.toString().replace(/,/g, ".");

	return valor;
}

function calculaTotalRecurso() {
	var quantidade	=	document.getElementsByName('eaaqtd')[0];
	var custeio		=	document.getElementsByName('eaavlrcusteio')[0];
	var capital		=	document.getElementsByName('eaavlrcapital')[0];
	//var total		=	document.getElementsByName('totalrecurso')[0];
	
	var restCusteio	=	document.getElementById("restante_custeio");
	var restCapital	=	document.getElementById("restante_capital");

	custeio = converteValorString(custeio.value);
	capital = converteValorString(capital.value);
	
	if(!quantidade.value) quantidade.value = 0;
	if(!custeio) custeio = 0;
	if(!capital) capital = 0;
	
	if(quantidade.value && custeio){
		var parcialRestCusteio 	= ( Number(valorRestanteCusteio) - (Number(custeio) * Number(quantidade.value)) );
		parcialRestCusteio 		= (parcialRestCusteio < 0) ? 0.00 : parcialRestCusteio;
		restCusteio.innerHTML = mascaraglobal( "[###.]###,##", parcialRestCusteio.toFixed(2) );
	}
	
	if(quantidade.value && capital){
		var parcialRestCapital 	= ( Number(valorRestanteCapital) - (Number(capital) * Number(quantidade.value)) );
		parcialRestCapital 		= (parcialRestCapital < 0) ? 0.00 : parcialRestCapital;
		restCapital.innerHTML = mascaraglobal( "[###.]###,##", parcialRestCapital.toFixed(2) );
	}
	//var totalParcial = ( (Number(custeio) + Number(capital)) * Number(quantidade.value) );
	//totalParcial = totalParcial.toFixed(2);
	
	//total.value = mascaraglobal("[###.]###,##", totalParcial);
}

function filtraItemFinanciavel(etaid)
{
	var td				= document.getElementById('item_financiavel');
	var select 			= document.getElementsByName('etiid')[0];
	var eaavlrcapital 	= document.getElementsByName('eaavlrcapital')[0];
	var eaavlrcusteio 	= document.getElementsByName('eaavlrcusteio')[0];
	
	select.disabled 			= true;
	select.options[0].selected 	= true;
	select.options[0].text 		= 'Carregando...';

	// desabilita/habilita os campos necessários
	/*eaavlrcapital.disabled 	= false;
	eaavlrcapital.className = 'normal';
	eaavlrcusteio.disabled 	= false;
	eaavlrcusteio.className = 'normal';*/

	var selectAcao = document.getElementsByName('etaid')[0];
	
	/*<?php if( (integer)$_SESSION['exercicio'] == 2010 ): ?>
	if( selectAcao.selectedIndex == 1 || selectAcao.selectedIndex == 2 )
	{
		eaavlrcapital.className = 'disabled';
		eaavlrcapital.disabled = true;
	}
	else
	{
		eaavlrcusteio.disabled = false;
		eaavlrcapital.disabled = false;
	}
	<?php elseif( (integer)$_SESSION['exercicio'] == 2011 ): ?>
	if( etaid == 7 )
	{
		eaavlrcusteio.disabled = false;
		eaavlrcapital.className = 'disabled';
		eaavlrcapital.disabled = true;
	}
	else if( etaid == 8 )
	{
		eaavlrcapital.disabled = false;
		eaavlrcusteio.className = 'disabled';
		eaavlrcusteio.disabled = true;
	}
	else
	{
		eaavlrcusteio.disabled = false;
		eaavlrcapital.disabled = false;
	}
	<?php endif; ?>*/
	
	if(etaid){
		var req = new Ajax.Request('pdeescola.php?modulo=eaprincipal/plano_acao&acao=A', {
							        method:     'post',
							        parameters: '&ajaxEtaid=' + etaid,
							        onComplete: function (res)
							        {							        	
										td.innerHTML = 'Itens Financiáveis<br />' + res.responseText;
							        }
							  });
	}
	
}

function filtraRecuperaItemFinanciavel(etaid, etiid) {
	var td		= document.getElementById('item_financiavel');
	var select 	= document.getElementsByName('etiid')[0];
	
	select.disabled 			= true;
	select.options[0].selected 	= true;
	select.options[0].text 		= 'Carregando...';
		
	var req = new Ajax.Request('pdeescola.php?modulo=eaprincipal/plano_acao&acao=A', {
							        method:     'post',
							        parameters: '&ajaxEtaid=' + etaid,
							        onComplete: function (res)
							        {							        	
										td.innerHTML = 'Itens Financiáveis<br />' + res.responseText;
										// Seleciona o item certo do combo a partir do 'etiid'
										var novoSelect 	= document.getElementsByName('etiid')[0];
										var selecionado	= "";
										
										for(var i=0; i<novoSelect.options.length; i++) {
											if(novoSelect.options[i].value == etiid)
												selecionado = novoSelect.options[i].index;
										}

										novoSelect.selectedIndex = selecionado;
							        }
							  });
}

function verificaTipoItens(etiid){
	var eaavlrcapital 	= document.getElementsByName('eaavlrcapital')[0];
	var eaavlrcusteio 	= document.getElementsByName('eaavlrcusteio')[0];
	var ano 	= document.getElementsByName('ano')[0];
	
	if(ano.value >= '2012'){
		var req = new Ajax.Request('pdeescola.php?modulo=eaprincipal/plano_acao&acao=A', {
								        method:     'post',
								        parameters: '&ajaxTipoItens=' + etiid,
								        onComplete: function (res)
								        {		
								        	if(res.responseText == '1'){
								        		eaavlrcusteio.disabled = true;
								        		eaavlrcapital.disabled = false;
								        		
								        		eaavlrcusteio.className = 'disabled';
								        		eaavlrcapital.className = 'normal';
								        	} else {
								        		eaavlrcusteio.disabled = false;
								        		eaavlrcapital.disabled = true;
								        		
								        		eaavlrcusteio.className = 'normal';
								        		eaavlrcapital.className = 'disabled';
								        	}
								        }
								  });
	}
}

function cancelarPlanoAcao() {
	// botões
	var btSalvar		=	document.getElementById("btSalvarPlanoAcao");
	// campos do form
	var acao	=	document.getElementsByName('etaid')[0];
	var item	=	document.getElementsByName('etiid')[0];
	var desc	=	document.getElementsByName('eaadescricao')[0];
	var qtd		=	document.getElementsByName('eaaqtd')[0];
	var custeio	=	document.getElementsByName('eaavlrcusteio')[0];
	var capital	=	document.getElementsByName('eaavlrcapital')[0];
	var total	=	document.getElementsByName('totalrecurso')[0];
	var eaaid	=	document.getElementsByName('eaaid')[0];
	
	// desabilita os botões para evitar erros do usuário
	btSalvar.disabled 	= true;
	
	// reseta os combos
	acao.selectedIndex = 0;
	item.selectedIndex = 0;
	
	// limpa os campos de texto
	desc.value 		= "";
	qtd.value 		= "";
	custeio.value 	= "";
	capital.value 	= "";
	total.value 	= "";
	eaaid.value		= "";
	
	// habilita os botões novamente
	btSalvar.disabled 	= false;
}

function alterarPlanoAcao(eaaid) {
	var hiddenEaaid	=	document.getElementsByName('eaaid')[0];
	var submetido	=	document.getElementsByName('submetido')[0];
	var form		=	document.getElementsByName('formPlanoAcao')[0];
	// botões
	var btSalvar	=	document.getElementById("btSalvarPlanoAcao");

	// desabilita os botões para evitar erros do usuário
	btSalvar.disabled 	= true;
	
	hiddenEaaid.value = eaaid;
	submetido.value = 0;
	form.submit();
}

function excluirPlanoAcao(eaaid) {
	if(confirm("Deseja realmente excluir este registro?")) {
		var excluir		=	document.getElementsByName('excluir')[0];
		var form		=	document.getElementsByName('formPlanoAcao')[0];
		// botões
		var btSalvar	=	document.getElementById("btSalvarPlanoAcao");
	
		// desabilita os botões para evitar erros do usuário
		btSalvar.disabled 	= true;
	
		excluir.value = eaaid;
		form.submit();
	}
}

<? 
if($eaaid) {
	echo "filtraRecuperaItemFinanciavel(".$etaid.", ".$etiid."); \n";
	echo "calculaTotalRecurso();\n";
	echo "verificaTipoItens($etiid);";
} 
?>

</script>