<?php 

// Verifica se as variáveis de sessão estão armazendas corretamente.
eaVerificaSessao();

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

if((integer)$vPerfilCadEscolaAcessivel > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == eaMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid']   );
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

// trata os dados submetidos
if( $_REQUEST["submetido"] ) {
	// verifica se há alguma entidade cadastrada
	$sql 	= "SELECT entid FROM pdeescola.eacentidade WHERE eacid = {$_SESSION['eacid']}";
	$existe = $db->carregar($sql);

	if( $existe ) {
		// exclui todas as entidades cadastradas
		$sql = "DELETE FROM pdeescola.eacentidade WHERE eacid = {$_SESSION['eacid']}";
		$db->executar($sql);
	}
	
	// grava, se houver, as parcerias com escolas públicas
	if( $_REQUEST["check_educacao_publica"] )
	{
		for($i=0; $i<count($_REQUEST["entid_publica"]); $i++)
		{
			$sql = "INSERT INTO
						pdeescola.eacentidade
							(eacid,
							 entid,
							 eteid)
					VALUES
							({$_SESSION['eacid']},
							 {$_REQUEST["entid_publica"][$i]},
							 2)";
			$db->executar($sql);
		}
		
		$sql = "UPDATE pdeescola.eacescolaacessivel SET eapossuiparceria = 't' WHERE eacid = ".$_SESSION['eacid'];
		$db->executar($sql);
	}
	// grava, se houver, as parcerias com escolas privadas
	if( $_REQUEST["check_educacao_privada"] )
	{
		for($i=0; $i<count($_REQUEST["entid_privada"]); $i++)
		{
			$sql = "INSERT INTO
						pdeescola.eacentidade
							(eacid,
							 entid,
							 eteid)
					VALUES
							({$_SESSION['eacid']},
							 {$_REQUEST["entid_privada"][$i]},
							 1)";
			$db->executar($sql);
		}
		
		$sql = "UPDATE pdeescola.eacescolaacessivel SET eapossuiparceria = 't' WHERE eacid = ".$_SESSION['eacid'];
		$db->executar($sql);
	}
	if( $_REQUEST["check_sem_parceria"] ) {
		$sql = "UPDATE pdeescola.eacescolaacessivel SET eapossuiparceria = 'f' WHERE eacid = ".$_SESSION['eacid'];
		$db->executar($sql);
	}
	
	if(count($_REQUEST['eqpid']) > 0){
		
		$sql = "delete from pdeescola.eacescolaquestao where eacid = '{$_SESSION['eacid']}'";
		$db->executar($sql);
		
		foreach($_REQUEST['eqpid'] as $eqpid){
			
			$sql = "insert into pdeescola.eacescolaquestao (eacid, eqpid) values ('{$_SESSION['eacid']}', '{$eqpid}')";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	$db->sucesso('eaprincipal/diagnostico');
}

echo "<br />";
echo montarAbasArray(carregaAbasEscolaAcessivel(), "/pdeescola/pdeescola.php?modulo=eaprincipal/diagnostico&acao=A");

$titulo = 'Escola Acessível';
$subtitulo = '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório';

monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAcessivel();

?>

<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>

<style>

.educacao {
	width:100%;
	background-color:#c0c0c0;
	border-style:solid;
	border-color:#000000;
	border-width:1px;
	padding-top:5px;
	padding-bottom:5px;
	font-weight:bold;
}

.educacao_entidades {
	width:100%;
	background-color:#f5f5f5;
	border-style:solid;
	border-color:#000000;
	border-width:1px;
	padding-top:10px;
	padding-bottom:10px;
	display:none;
	border-top-style: none; 
}

</style>

<form id="formDiagnostico" action="" method="post">
<input type="hidden" name="submetido" value="1" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="4" cellPadding="4" align="center" border="0">
	<tr>
		<td style="text-align:left; font-weight:bold; color:red;">
			Existência de parceria com instituição de educação especial:
		</td>
	</tr>
	<tr>
		<td>
			<div id="educacao_publica" class="educacao">
				<input type="checkbox" id="check_educacao_publica" disabled="disabled" name="check_educacao_publica" /> Sim, com instituição de educação especial pública
			</div>
			<div id="educacao_publica_entidades" class="educacao_entidades">
			<div style="padding-left:10px;">
				<table class="listagem" cellpadding="1" cellspacing="1" width="600px" id="tabela_escolas_publicas">
					<thead>
						<tr>
							<th align="center" width="20%">Ação</th>
							<th align="center" width="80%">Escola</th>
						</tr>
					</thead>
					<?
					// recupera os dados, se houver, das parcerias públicas da escola
					$sql = "SELECT
								eac.entid,
								ent.entnome
						    FROM
						   		pdeescola.eacentidade eac
						   	INNER JOIN
						   		entidade.entidade ent ON ent.entid = eac.entid
						   	WHERE
						   		eac.eacid = {$_SESSION['eacid']}
						   		AND eac.eteid = 2";
					$dados = $db->carregar($sql);
					
					if( $dados ) {
						// força o componente...
						echo "<script>	
								$('#educacao_publica_entidades').slideDown('fast', function() {});
								$('#check_educacao_publica').attr('checked', true);
							  </script>";
						
						for($i=0; $i<count($dados); $i++) {
							$cor = ($i%2) ? "#e0e0e0" : "#f4f4f4";
							
							echo '<tr style="background-color:'.$cor.';" id="escola_'.$dados[$i]["entid"].'">
									<td style="text-align:center;">
										<!--<img style="cursor:pointer;" title="Excluir escola" src="../imagens/excluir.gif" onclick="excluiEscola(\'publica\', this.parentNode.parentNode.rowIndex);" />-->
										<img style="cursor:pointer;" title="Excluir escola" src="../imagens/excluir_01.gif" />
										<input type="hidden" name="entid_publica[]" value="'.$dados[$i]["entid"].'" />
									</td>
									<td style="text-align:center;">
										'.$dados[$i]["entnome"].'
									</td>
								  </tr>';
						}
					}
					?>
				</table>
				<br />
				<!--<a style="cursor:pointer;" onclick="inserirNovaEscola('publica');"><img src="../imagens/gif_inclui.gif" /> Inserir nova escola</a>-->
				<a style="cursor:pointer;"><img src="../imagens/gif_inclui_d.gif" /> Inserir nova escola</a>
			</div>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div id="educacao_privada" class="educacao">
				<input type="checkbox" id="check_educacao_privada" name="check_educacao_privada" disabled="disabled" /> Sim, com instituição de educação especial privada sem fins lucrativos
			</div>
			<div id="educacao_privada_entidades" class="educacao_entidades">
			<div style="padding-left:10px;">
				<table class="listagem" cellpadding="1" cellspacing="1" width="600px" id="tabela_escolas_privadas">
					<thead>
						<tr>
							<th align="center" width="20%">Ação</th>
							<th align="center" width="80%">Escola</th>
						</tr>
					</thead>
					<?
					// recupera os dados, se houver, das parcerias privadas da escola
					$sql = "SELECT
								eac.entid,
								ent.entnome
						    FROM
						   		pdeescola.eacentidade eac
						   	INNER JOIN
						   		entidade.entidade ent ON ent.entid = eac.entid
						   	WHERE
						   		eac.eacid = {$_SESSION['eacid']}
						   		AND eac.eteid = 1";
					$dados = $db->carregar($sql);
					
					if( $dados ) {
						// força o componente...
						echo "<script>	
								$('#educacao_privada_entidades').slideDown('fast', function() {});
								$('#check_educacao_privada').attr('checked', true);
							  </script>";
						
						for($i=0; $i<count($dados); $i++) {
							$cor = ($i%2) ? "#e0e0e0" : "#f4f4f4";
							
							echo '<tr style="background-color:'.$cor.';" id="escola_'.$dados[$i]["entid"].'">
									<td style="text-align:center;">
										<!--<img style="cursor:pointer;" title="Excluir escola" src="../imagens/excluir.gif" onclick="excluiEscola(\'privada\', this.parentNode.parentNode.rowIndex);" />-->
										<img style="cursor:pointer;" title="Excluir escola" src="../imagens/excluir_01.gif" />
										<input type="hidden" name="entid_privada[]" value="'.$dados[$i]["entid"].'" />
									</td>
									<td style="text-align:center;">
										'.$dados[$i]["entnome"].'
									</td>
								  </tr>';
						}
					}
					?>
				</table>
				<br />
				<!--<a style="cursor:pointer;" onclick="inserirNovaEscola('privada');"><img src="../imagens/gif_inclui.gif" /> Inserir nova escola</a>-->
				<a style="cursor:pointer;"><img src="../imagens/gif_inclui_d.gif" /> Inserir nova escola</a>
			</div>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div id="sem_parceria" class="educacao">
				<input type="checkbox" id="check_sem_parceria" disabled="disabled" name="check_sem_parceria" /> Não tem parceria com instituição de educação especial
				<?
				// verifica se há alguma escola cadastrada para qualquer uma das parcerias
				$sql = "SELECT
							eapossuiparceria
					    FROM
					   		pdeescola.eacescolaacessivel
					   	WHERE
					   		eacid = {$_SESSION['eacid']}";
				$dados = $db->carregar($sql);
				// se não houver marca a checkbox da opção que não contempla parceria
				if( !$dados[0]["eapossuiparceria"] || $dados[0]["eapossuiparceria"] == 'f' ) echo "<script> $('#check_sem_parceria').attr('checked', true); </script>";
				?>
			</div>
		</td>
	</tr>
	<tr id="titulo_acoes" style="display:none;">
		<td style="text-align:left; font-weight:bold; color:red;">
			A parceria entre a escola e a instituição de educação especial, compreende:
		</td>
	</tr>
	<tr id="opcoes_acoes" style="display:none;">
		<td>
			<!-- input type="checkbox" /> <b>A oferta do atendimento aducacional especializado complementar à escolarização</b><br />
			<input type="checkbox" /> <b>A promoção da formação continuada de professores</b><br />
			<input type="checkbox" /> <b>A disponibilização de recursos, serviços e materiais didáticos e pedagógicos para a acessibilidade</b><br />
			<input type="checkbox" /> <b>A promoção de ações intersetoriais para a avaliação do plano de atendimento educacional especializado - AEE do aluno</b -->
			<?php
			
			$sql = "select 
						eqpid,
						eqpdescricao 
					from
						pdeescola.eacquestaoparceria 
					where
						eqpstatus = 'A' 
					order by
						eqpdescricao asc";
			$arQuestoes = $db->carregar($sql);
			
			$sql = "select 
						eqpid 
					from
						pdeescola.eacescolaquestao 
					where
						eacid = '{$_SESSION['eacid']}'";
			$arQuestao = $db->carregar($sql);
			
			$arQuestao = $arQuestao ? $arQuestao : array();
			
			$arEqpids = array();
			if($arQuestao)
			{
				foreach($arQuestao as $arEqpid)
				{
					$arEqpids[] = $arEqpid['eqpid'];
				}			
			}
			
			if($arQuestoes)
			{
				foreach($arQuestoes as $questao){
					
					$marcar = in_array($questao['eqpid'], $arEqpids) ? "checked" : "";								
					echo '<input type="checkbox" disabled="disabled" name="eqpid[]" value="'.$questao['eqpid'].'" '.$marcar.'><b>'.$questao['eqpdescricao'].'</b><br>';
				}
			}
			
			?>
		</td>
	</tr>
	<tr>
		<td style="text-align:center;">
			<input type="button" value="Salvar" id="btSalvar" onclick="salvarDiagnostico();" disabled="disabled" />
			<input type="button" value="Voltar" id="btCancelar" onclick="cancelarDiagnostico();" />
		</td>
	</tr>
</table>
</form>

<script>

<?
/*** Desabilita a opção de salvar, se necessário ***/
$perfis = arrayPerfil();
 
if( !in_array(PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL, $perfis) && !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
{
	echo "$('#btSalvar').attr('disabled', true);";
}
?>

/*** INICIO - se clicar na parceria com instituições públicas - INICIO ***/
$('#check_educacao_publica').click(
	function() {
		// está checado
		if( $('#check_educacao_publica').is(':checked') ) {
			// se a opção de sem parceria está marcada
			if( $('#check_sem_parceria').is(':checked') ) {
				// se o usuário quiser desmarcar a opção de sem parceria
				if( confirm('Esta ação irá desmarcar a opção de não contemplar parceria.\nDeseja continuar?') ) {
					$('#educacao_publica_entidades').slideDown('fast', function() {});
					$('#check_sem_parceria').attr('checked', false);
				} 
				// se o usuário não quiser desmarcar a opção de sem parceria
				else {
					$('#check_educacao_publica').attr('checked', false);
				}
			}
			// se a opção de sem parceria está desmarcada
			else {
				$('#educacao_publica_entidades').slideDown('fast', function() {});
			}
		}
		// não está checado
		else {
			// se existir alguma escola cadastrada
			if( existeEscolaLista('publica') ) {
				// se o usuário decidir excluir todas as escolas da lista
				if(confirm('Desmarcar esta parceria irá excluir todas as entidades que foram associadas.\nDeseja continuar?')) {
					excluiEscolaLista('publica');
					$('#educacao_publica_entidades').slideUp('fast', function() {});
				} 
				// se o usuário decidir não excluir todas as escolas da lista
				else {
					$('#check_educacao_publica').attr('checked', true);
				}
			}
			// se não existir qualquer escola cadastrada
			else {
				$('#educacao_publica_entidades').slideUp('fast', function() {});
			}
		}

		mostraOpcoesAcoes();
	}
);
/*** FIM - se clicar na parceria com instituições públicas - FIM ***/



/*** INICIO - se clicar na parceria com instituições privadas - INICIO ***/
$('#check_educacao_privada').click(
	function() {
		// está checado
		if( $('#check_educacao_privada').is(':checked') ) {
			// se a opção de sem parceria está marcada
			if( $('#check_sem_parceria').is(':checked') ) {
				// se o usuário quiser desmarcar a opção de sem parceria
				if( confirm('Esta ação irá desmarcar a opção de não contemplar parceria.\nDeseja continuar?') ) {
					$('#educacao_privada_entidades').slideDown('fast', function() {});
					$('#check_sem_parceria').attr('checked', false);
				} 
				// se o usuário não quiser desmarcar a opção de sem parceria
				else {
					$('#check_educacao_privada').attr('checked', false);
				}
			}
			// se a opção de sem parceria está desmarcada
			else {
				$('#educacao_privada_entidades').slideDown('fast', function() {});
			}
		}
		// não está checado
		else {
			// se existir alguma escola cadastrada
			if( existeEscolaLista('privada') ) {
				// se o usuário decidir excluir todas as escolas da lista
				if(confirm('Desmarcar esta parceria irá excluir todas as entidades que foram associadas.\nDeseja continuar?')) {
					excluiEscolaLista('privada');
					$('#educacao_privada_entidades').slideUp('fast', function() {});
				} 
				// se o usuário decidir não excluir todas as escolas da lista
				else {
					$('#check_educacao_privada').attr('checked', true);
				}
			}
			// se não existir qualquer escola cadastrada
			else {
				$('#educacao_privada_entidades').slideUp('fast', function() {});
			}
		}

		mostraOpcoesAcoes();
	}
);
/*** FIM - se clicar na parceria com instituições privadas - FIM ***/



/*** INICIO - se clicar na opção de não comtemplar parceria - INICIO ***/
$('#check_sem_parceria').click(
	function() {
		// se está checado
		if( $('#check_sem_parceria').is(':checked') ) {
			
			// se a opção de educação pública OU privada estiver marcada
			if( $('#check_educacao_publica').is(':checked') || $('#check_educacao_privada').is(':checked') ) {
				
				// se o usuário escolher apagar as escolas que foram selecionadas para as parcerias públicas e/ou privadas
				if( confirm('Marcar esta opção irá excluir todas as entidades que foram associadas as parcerias com instituições de ensino acima.\nDeseja continuar?') ) {
					
					// apagas a(s) lista(s) de escolas
					if( existeEscolaLista('publica') ) excluiEscolaLista('publica');
					if( existeEscolaLista('privada') ) excluiEscolaLista('privada');
					
					// esconde as divs
					$('#educacao_publica_entidades').slideUp('fast', function() {});
					$('#educacao_privada_entidades').slideUp('fast', function() {});
					
					// retira o check
					$('#check_educacao_publica').attr('checked', false);
					$('#check_educacao_privada').attr('checked', false);
				} 
				// se o usuário não quiser apagar as escolas que foram selecionadas para as parcerias públicas e/ou privadas
				else {
					$('#check_sem_parceria').attr('checked', false);
				}
			}
		}

		mostraOpcoesAcoes();
	}
);
/*** FIM - se clicar na opção de não comtemplar parceria - FIM ***/

// verifica se os checkbox devem ser exibidos ou não...
mostraOpcoesAcoes();

function mostraOpcoesAcoes() {
	var tituloAcoes 		= 	document.getElementById("titulo_acoes");
	var opcoesAcoes 		= 	document.getElementById("opcoes_acoes");
	var checkPublica		= 	document.getElementsByName("check_educacao_publica")[0];
	var checkPrivada		= 	document.getElementsByName("check_educacao_privada")[0];

	if(checkPublica.checked || checkPrivada.checked)
	{
		tituloAcoes.style.display = "table-row";
		opcoesAcoes.style.display = "table-row";
	}
	else 
	{
		tituloAcoes.style.display = "none";
		opcoesAcoes.style.display = "none";
	}
}

function cancelarDiagnostico() {
	var btSalvar 	= document.getElementById("btSalvar");
	var btCancelar 	= document.getElementById("btCancelar");

	btSalvar.disabled 	= true;
	btCancelar.disabled = true;

	location.href = "pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A";
}

function salvarDiagnostico() {
	var form				=	document.getElementById("formDiagnostico");
	var btSalvar 			= 	document.getElementById("btSalvar");
	var btCancelar 			= 	document.getElementById("btCancelar");
	var checkPublica		= 	document.getElementsByName("check_educacao_publica")[0];
	var checkPrivada		= 	document.getElementsByName("check_educacao_privada")[0];
	var checkSemParceria	= 	document.getElementsByName("check_sem_parceria")[0];
	
	btSalvar.disabled 	= true;
	btCancelar.disabled = true;

	// valida se pelo menos uma parceria foi selecionada
	if(!checkPublica.checked && !checkPrivada.checked && !checkSemParceria.checked) {
		alert("Pelo menos uma opção deve ser selecionada.");
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	// valida se existe pelo menos uma escola cadastrada para a parceria pública selecionada
	if(checkPublica.checked) {
		var tabela 		= 	document.getElementById("tabela_escolas_publicas");
		
		if(tabela.rows.length == 1) {
			alert("Você deve inserir pelo menos uma escola na parceira\ncom instituição de educação pública.");
			btSalvar.disabled 	= false;
			btCancelar.disabled = false;
			return;
		}
	}
	// valida se existe pelo menos uma escola cadastrada para a parceria privada selecionada
	if(checkPrivada.checked) {
		var tabela 		= 	document.getElementById("tabela_escolas_privadas");
		
		if(tabela.rows.length == 1) {
			alert("Você deve inserir pelo menos uma escola na parceira\ncom instituição de educação privada.");
			btSalvar.disabled 	= false;
			btCancelar.disabled = false;
			return;
		}
	}
	
	// Valida Questões
	if(!checkSemParceria.checked){
	
		if(document.getElementsByName("eqpid[]")){
		
			var totEqpid = document.getElementsByName("eqpid[]").length;
			mostraAlert = false;
			for(x=0;x<totEqpid;x++){
			
				if(document.getElementsByName("eqpid[]")[x].checked){
				
					mostraAlert = true;
					
				} 	
			}
			
			if(mostraAlert == false){
			
				alert("É obrigatório a marcação de pelo menos uma resposta para a descrição do que a parceria compreende.");
				btSalvar.disabled 	= false;
				btCancelar.disabled = false;
				return false;		
			}
		}
	}

	// se estiver tudo ok, submete os dados
	form.submit();
}

function inserirNovaEscola(tipo) {
	var janela = window.open( 'pdeescola.php?modulo=eaprincipal/popInsereEscolaDiagnostico&acao=A&tipo='+tipo, 'relatorio', 'width=400,height=50,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function excluiEscola(tipo, index) {
	if(tipo == "publica") {
		var tabela = document.getElementById("tabela_escolas_publicas");
	} else {
		var tabela = document.getElementById("tabela_escolas_privadas");
	}

	tabela.deleteRow(index);
}

function existeEscolaLista(tipo) {
	if(tipo == "publica") {
		var tabela = document.getElementById("tabela_escolas_publicas");
	} else {
		var tabela = document.getElementById("tabela_escolas_privadas");
	}

	if(tabela.rows.length == 1)
		return false;
	else
		return true;
}

function excluiEscolaLista(tipo) {
	if(tipo == "publica") {
		var tabela = document.getElementById("tabela_escolas_publicas");
	} else {
		var tabela = document.getElementById("tabela_escolas_privadas");
	}

	while( tabela.rows.length > 1) {
		tabela.deleteRow(1);
	}
}

</script>