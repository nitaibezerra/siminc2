<?php
include_once APPRAIZ . 'includes/workflow.php';

$entid	   = $_SESSION['academico']['entid'];
$docid	   = pegarDocid( $_REQUEST['greid'] );

$autoriazacaoconcursos = new autoriazacaoconcursos();
$c = new CursosEdital();

//dbg( $c->carregaGrupoCursoEquivalencia(array("greid" => $_REQUEST['greid']), array("tipoEquivalencia" => "E")), 1 );

switch ( $_REQUEST['evento'] ){
	case 'salvar':
		$where = array();
		if ( $_REQUEST['greid'] ){
			$where['greid'] = $_REQUEST['greid']; 
		}
		// Retorna ID do "grupoequivalencia", ou false, em caso de falha
		$retorno = $c->manterCursoequivalencia( $_POST, $where );
		if ( $retorno ){
			$greid 	  = $_REQUEST['greid'] ? $_REQUEST['greid'] : $retorno;
			$paramUrl = "&greid=" . $greid;
			
			criarDocumento( $greid );
			$db->sucesso($_REQUEST['modulo'], $paramUrl);
		}else{
			redir('', 'Falha na operação!\nHouve alguma incompatibilidade nos dados enviados.');
		}
	break;
}


include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Cadatro de Equivalência', '' );
// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entid);
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<form name="formulario" id="formulario" action="" method="post" onsubmit="javascript: return validaForm();">	
<input type="hidden" name="evento" value="salvar">
<input type="hidden" name="greid" value="<?=$_REQUEST['greid'] ?>">
<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellspacing="1" cellpadding="3" style="border-bottom:none; border-top: 0px;">
	<tr>
		<td>
			<table align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none; width: 60%;">
				<tr>
					<td valign="top" width="370px">
						<fieldset style="height: 155px; background: #ffffff;">
						<legend>PREVISTOS</legend>	
							<div style="margin: 0 auto; padding: 0; height: 150px; width: 350px; overflow: auto; background-color: #ffffff; border: none;" >						
								<? 
									$param = ($_REQUEST['greid']) ? array("greid" => $_REQUEST['greid']) : null;
									$c->listaCheckCursoEquivalencia($param, array("tipoLista" => "P")); 
								?>	
							</div>
						</fieldset>
					</td>
					<td valign="top" width="360px">
						<fieldset style="height: 155px; background: #ffffff;">
						<legend>EXECUTADOS</legend>	
							<div style="margin: 0 auto; padding: 0; height: 150px; width: 350px; overflow: auto; background-color: #ffffff; border: none;" >
								<?
									$param = ($_REQUEST['greid']) ? array("greid" => $_REQUEST['greid']) : null;
									$c->listaCheckCursoEquivalencia($param, array("tipoLista" => "E")); 
								?>	
							</div>
						</fieldset>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<fieldset>
						<legend>EQUIVALÊNCIA</legend>
							<table align="center" cellspacing="1" cellpadding="3" style="border-bottom:none; width: 100%;">
								<tr>
									<td valign="top" width="360px">
										<fieldset style="height: 155px; background: #ffffff;">
										<legend>PREVISTOS</legend>	
											<div id="lista_pactuado" style="height: 150px; overflow: auto;">						
												<? 
												$listPac = array();
												if ( $_REQUEST['greid'] ){
													$listPac = $c->carregaGrupoCursoEquivalencia( array("greid" => $_REQUEST['greid']), array("tipoEquivalencia" => "P", "tipoLista" => "text") );
													echo implode("<br/>", $listPac);													
												}
												?>	
											</div>
										</fieldset>
									</td>
									<td valign="top" width="360px">
										<fieldset style="height: 155px; background: #ffffff;">
										<legend>EXECUTADOS</legend>	
											<div id="lista_executado" style="height: 150px; overflow: auto;">
												<? 
												$listExec = array();
												if ( $_REQUEST['greid'] ){
													$listExec = $c->carregaGrupoCursoEquivalencia( array("greid" => $_REQUEST['greid']), array("tipoEquivalencia" => "E", "tipoLista" => "text") );
													echo implode("<br/>", $listExec);													
												}
												?>	
											</div>
										</fieldset>
									</td>
								</tr>
							</table>
						</fieldset>
					</td>
				</tr>
			</table>
		</td>
<? 
	if ( $docid ){
?>	
		<td valign="top" align="right">
<? 
		// Barra de estado atual e ações e Historico
		wf_desenhaBarraNavegacao( $docid, array( 'greid' => $_REQUEST['greid'] ) );	
?>		
		</td>
<?		
	}	
?>	
	</tr>
	<tr>
		<td colspan="2" class="SubTituloEsquerda">
	  		<center>
<? 
		if ( (pegarEstadoAtual( $docid ) != ESDID_APROVADO && pegarEstadoAtual( $docid ) != ESDID_EQUIVALENCIA_FINALIZADA) 
				|| 
			 (pegarEstadoAtual( $docid ) == ESDID_ANALISE_MEC 
			 && 
			 !academico_possui_perfil( array(PERFIL_MECCADBOLSAS, PERFIL_ADMINISTRADOR) )) ):
?>	  		
			<input name="salvar" type="submit" value="Salvar">
			&nbsp;
<?
		endif;
?>			
			<input name="voltar" type="button" value="Voltar" onclick="location.href='?modulo=principal/cursosevagas/listaEquivalencia&acao=A'">			
			</center>
		</td>
	</tr>
</table>	
</form>
<script type="text/javascript">
var totPactuado  = <?=count( $listPac ); ?>;
var totExecutado = <?=count( $listExec ); ?>;

function montaPactuados ( checkClick ){
	var text 	= '';
	var htmlIni = $("#lista_pactuado").html(); 
	totPactuado = 0;
	
	$("[name^='curidpactuado']").each( function (){
		if ( $(this).attr('checked') ){
			totPactuado++;
			if ( totExecutado > 1 && totPactuado > 1 ){
				$(checkClick).attr('checked', false);
				text = htmlIni;
				alert('No caso de vários cursos Executados, só pode haver um curso Pactuado!');
				totPactuado--;
				return;
			}
			text += $("[for^='" + $(this).attr('id') + "']").text() + '<br/>';
		}
	});
	
	$("#lista_pactuado").html('');
	$("#lista_pactuado").html( text );
}

function montaExecutados ( checkClick ){
	var text 	 = '';
	var htmlIni  = $("#lista_executado").html(); 
	totExecutado = 0
	
	$("[name^='curidexecutado']").each( function ( i ){
		if ( $(this).attr('checked') ){
			totExecutado++;
			if ( totExecutado > 1 && totPactuado > 1 ){
				$(checkClick).attr('checked', false);
				text = htmlIni;
				alert('No caso de vários cursos Previstos, só pode haver um curso Executado!'); 
				totExecutado--; 
				return;
			}
			text += $("[for^='" + $(this).attr('id') + "']").text() + '<br/>';
		}
	});
	
	$("#lista_executado").html('');
	$("#lista_executado").html( text );
}

function validaForm (){
	var retorno = false;
	
	$("[name^='curidpactuado']").each( function (){
		if ( $(this).attr('checked') ){
			retorno = true;
			return;
		}	
	});
	
	if ( retorno  ){
		retorno = false;
		$("[name^='curidexecutado']").each( function (){
			if ( $(this).attr('checked') ){
				retorno = true;
				return;
			}	
		});
	}
	
	if ( !retorno ){
		alert('Para salvar a equivalência, é necessário assinalar cursos PREVISTOS e EXECUTADOS!');
		return false;
	}
	return true;	
}
</script>