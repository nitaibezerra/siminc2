<?php 

if( !$_SESSION['catalogo']['curid'] ){
	$permissoes['gravar'] = true;
}

// formulário de pesquisa
function monta_checkbox_status_pesquisa( $dados ){
	
	global $db,$permissoes;
	
	$sql = "(SELECT 
				'A' as codigo, 
				'Ativo' as descricao)
			UNION 
			(SELECT
				'I' as codigo,
				'Inativo' as descricao)";
	$marcados = $_POST['curstatus'];
	$db->monta_checkbox('curstatus[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

function monta_combo_areaTematica_pesquisa( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				ateid as codigo, 
				ateid||' - '||atedesc as descricao
			FROM 
				catalogocurso.areatematica
			WHERE
				atestatus = 'A'
			ORDER BY 
				atedesc";
	$db->monta_combo('ateid', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Área Temática', '', 'N', 'ateid', '', $_POST['ateid']); 
}

function monta_combo_nivelCurso_pesquisa( $dados ){
	
	global $db,$permissoes,$_POST;
	
	$sql = "SELECT 
				ncuid as codigo, 
				ncuid||' - '||ncudesc as descricao
			FROM 
				catalogocurso.nivelcurso
			WHERE
				ncustatus = 'A'";
	$db->monta_combo('ncuid', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Nivel do Curso', '', 'N', 'ncuid', '', $_POST['ncuid']); 
}

function monta_checkbox_rede_pesquisa( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				redid as codigo, 
				reddesc as descricao
			FROM 
				catalogocurso.rede
			WHERE
				redstatus = 'A'";
	$marcados = $_POST['redid'];
	$db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

//formulários de cadastro
 
function monta_checkbox_rede( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				redid as codigo, 
				reddesc as descricao
			FROM 
				catalogocurso.rede
			WHERE
				redstatus = 'A'";
	$marcados = $dados['redid'];
	$db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

function monta_checkbox_status( $dados ){
	
	global $db,$permissoes;
	
	$sql = "(SELECT 
				'A' as codigo, 
				'Ativo' as descricao)
			UNION 
			(SELECT
				'I' as codigo,
				'Inativo' as descricao)";
	$marcados = $_POST['redid'];
	$db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

function monta_checkbox_modalidade( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				pk_cod_mod_ensino as codigo, 
				no_mod_ensino as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_mod_ensino";
	
	$marcados = $dados['cod_mod_ensino'];
	$db->monta_checkbox('cod_mod_ensino[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

function monta_checkboxcheckbox_modalidadeCurso( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				modid as codigo, 
				moddesc as descricao
			FROM 
				catalogocurso.modalidadecurso
			WHERE
				modstatus = 'A' ";
	
	$marcados = $dados['modid'];
	$db->monta_checkbox('modid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); 
}

function monta_combo_areaTematica( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				ateid as codigo, 
				ateid||' - '||atedesc as descricao
			FROM 
				catalogocurso.areatematica
			WHERE
				atestatus = 'A'
			ORDER BY 
				atedesc";
	$db->monta_combo('ateid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Área Temática', '', 'S', 'ateid'); 
}

function monta_combo_nivelCurso( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				ncuid as codigo, 
				ncuid||' - '||ncudesc as descricao
			FROM 
				catalogocurso.nivelcurso
			WHERE
				ncustatus = 'A'";
	$db->monta_combo('ncuid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Nivel do Curso', '', 'S', 'ncuid'); 
}

function monta_combo_localizacaoEscola( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				lesid as codigo, 
				lesdesc as descricao 
			FROM 
				catalogocurso.localizacaoescola
			WHERE
				lesstatus = 'A'";
	$db->monta_combo('lesid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Localização da Escola', '', 'S', 'lesid'); 
}

function monta_combo_localizacaoDiferenciadaEscola( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				ldeid as codigo, 
				ldedesc as descricao 
			FROM 
				catalogocurso.localizacaodiferenciadaescola
			WHERE
				ldestatus = 'A'";
	$db->monta_combo('ldeid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Localização Diferenciada da Escola', '', 'S', 'ldeid'); 
}

function monta_combo_modalidadeCurso( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				modid as codigo, 
				moddesc as descricao
			FROM 
				catalogocurso.modalidadecurso
			WHERE
				modstatus = 'A' ";
	$db->monta_combo('modid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Modalidade do Curso', '', 'S', 'modid'); 
}

function monta_radio_uniMedMonitora( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				uteid as codigo, 
				utedesc as descricao
			FROM 
				catalogocurso.unidadetempo
			WHERE
				utestatus = 'A' ";
	$uteid = $uteid ? $uteid : 1;
	$db->monta_radio('uteid',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_multi( $dados ){
	
	global $db,$permissoes;
	
	$sql = "(SELECT 
				't' as codigo, 
				'Sim' as descricao)
			UNION 
			(SELECT
				'f' as codigo,
				'Não' as descricao)
			ORDER BY
				2 DESC";
	$cursalamulti = $cursalamulti ? $cursalamulti : 'f';
	$db->monta_radio('cursalamulti',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_status( $dados ){
	
	global $db,$curstatus,$permissoes;
	
	$sql = "(SELECT 
				'A' as codigo, 
				'Ativo' as descricao)
			UNION 
			(SELECT
				'I' as codigo,
				'Inativo' as descricao)";
	$curstatus = $curstatus ? $curstatus : 'A';
	$db->monta_radio('curstatus',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op,Array("obrigatorio"=>true));
}

function monta_radio_bolsistas( $dados ){
	
	global $db,$eqcbolsista,$permissoes;
	
	$sql = "(SELECT 
				't' as codigo, 
				'Sim' as descricao)
			UNION 
			(SELECT
				'f' as codigo,
				'Não' as descricao)
			ORDER BY
				2 DESC";
	$eqcbolsista = $eqcbolsista ? $eqcbolsista : 'f';
	$db->monta_radio('eqcbolsista',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_ofertaNacional( $dados ){
	
	global $db,$curofertanacional,$permissoes;
	
	$sql = "(SELECT 
				'true' as codigo, 
				'Sim' as descricao)
			UNION 
			(SELECT
				'false' as codigo,
				'Não' as descricao)
			ORDER BY
				2 DESC";
	$curofertanacional = $curofertanacional == 't' ? 'true' : 'false';
	$db->monta_radio('curofertanacional',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_combo_resp( $dados ){
	
	global $db,$permissoes;
	
	if( !$db->testa_superuser() ){
		$arrCoords = recuperaCoordenacaoResponssavel();
	}
	
//	$sql = "SELECT 
//				co_interno_uorg as codigo, 
//				sg_unidade_org||' - '||no_unidade_org as descricao
//			FROM 
//				siorg.tb_seo_unidade_org
//			WHERE
//				SUBSTRING(co_unidade_org FROM 1 FOR 1) = '0'";
//  $db->monta_combo('co_interno_uorg', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '', 'S', 'co_interno_uorg');
	$sql = "SELECT 
				coordid as codigo, 
				coordsigla||' - '||coorddesc as descricao 
			FROM 
				catalogocurso.coordenacao
			WHERE
				coordstatus = 'A'
			".(is_array($arrCoords) && count($arrCoords)>0   ? " AND coordid in (".implode(",",$arrCoords).")"  : "")."
			ORDER BY
				coordsigla";
	$db->monta_combo('coordid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '', 'S', 'coordid'); 
}

function listaCursos( $request ){
	
	global $db;
	
	$arrPflcods = Array();
	$arrCurids  = Array();
	$arrCoords  = Array();
	$pflcods    = pegaPerfis($_SESSION['usucpf']);
	if( !$db->testa_superuser() || in_array(PERFIL_CONSULTA,$pflcods) || in_array(PERFIL_ADMINISTRADOR,$pflcods) ){
		$arrCoords = recuperaCoordenacaoResponssavel();
		$arrCurids = recuperaCursoResponssavel();
		array_push($arrCoords,'0');
		array_push($arrCurids,'0');
		$pflcod     = pegaPerfil($_SESSION['usucpf']);
		$arrPflcods = Array(PERFIL_COORDENADOR,
							PERFIL_GESTOR);
	}
	
	$where = Array();
	
	if($request['curstatus']){
		array_push($where,"curstatus in ('".implode("','",$request['curstatus'])."')");
	}
	
	if($request['ateid']){
		array_push($where,"a.ateid = ".$request['ateid']."");
	}
	if($request['curid']){
		array_push($where,"c.curid = ".$request['curid']."");
	}
	
	if($request['curdesc']){
		array_push($where,"curdesc ilike '%".$request['curdesc']."%'");
	}
	
	if($request['ncuid']){
		array_push($where,"nc.ncuid = ".$request['ncuid']."");
	}

	if($request['coordid']){
		array_push($where,"c.coordid = ".$request['coordid']."");
	}
	
	if($request['redid']){
		array_push($where,"redid in (".implode(",",$request['redid']).")");
	}
	
	if($request['dt1']){
		$request['dt1'] = explode("/",$request['dt1']);
		$request['dt1'] = $request['dt1'][2]."/".$request['dt1'][1]."/".$request['dt1'][0];
		array_push($where,"curdtinclusao >= '".$request['dt1']."'");
	}
	
	if($request['dt2']){
		$request['dt2'] = explode("/",$request['dt2']);
		$request['dt2'] = $request['dt2'][2]."/".$request['dt2'][1]."/".$request['dt2'][0];
		array_push($where,"curdtinclusao >= '".$request['dt2']."'");
	}
	
	$sql = "SELECT DISTINCT
				'<center><img border=\"0\" align=\"absmiddle\" onclick=\"imprimirCurso('||c.curid||')\" title=\"Imprimir\" style=\"cursor: pointer\" src=\"../imagens/print.gif\"></center>' as imprimir,
				'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso.php?modulo=principal/cadCatalogo&acao=A&curid='|| c.curid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\"></center>' as alterar,
				".( $db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR,$pflcods) ? "'<center><img border=\"0\" align=\"absmiddle\" onclick=\"excluirCurso('|| c.curid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>'" : "' - '" )."
				 as excluir,
				CASE WHEN curstatus = 'A'
					THEN 'Ativo'
					ELSE 'Inativo'
				END as status,
				atedesc as areatematica,
				c.curid,
				c.curdesc||' ( versão '||curversao||' )' as curso, 
				ncudesc as nivelcurso,
				esd.esddsc,
				to_char(curdtinclusao,'DD/MM/YYYY') as data
			FROM 
				catalogocurso.curso c
			INNER JOIN workflow.documento doc ON doc.docid = c.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
			INNER JOIN catalogocurso.areatematica  a ON a.ateid = c.ateid
			INNER JOIN catalogocurso.cursorede     r ON r.curid = c.curid 
			INNER JOIN catalogocurso.nivelcurso nc ON nc.ncuid = c.ncuid
			LEFT  JOIN catalogocurso.usuarioresponsabilidade ur1 ON ur1.coordid = c.coordid
			LEFT  JOIN catalogocurso.usuarioresponsabilidade ur2 ON ur2.curid = c.curid
			WHERE
				curstatus = 'A' 
				".(count($where)>0 ? "AND ".implode(" AND ",$where) : "");
//				.(in_array($pflcod,$arrPflcods) ? "AND ( ur1.usucpf = '".$_SESSION['usucpf']."' )":"");
//				.($pflcod == PERFIL_COORDENADOR && count($arrCurids)>1 ? "AND c.curid in (".implode(",",$arrCurids).")" :"").
//				.(is_array($arrCoords) && count($arrCoords) > 1 ? "AND c.coordid in (".implode(",",$arrCoords).")" : "");
//	ver($sql);
	$cursos = $db->carregar($sql, null,3600);
	$cabecalho = array("&nbsp;", "&nbsp;", "&nbsp;", "Status", "Área Temática", "Código Curso", "Nome do Curso", "Nivel Curso", "Situação", "Data de Criação");
	$db->monta_lista_array($cursos, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

function combo_popup_etapaEnsino($post){
	
	global $db,$cod_etapa_ensino,$permissoes;
	
	$sql = "SELECT 
				pk_cod_etapa_ensino as codigo, 
				no_etapa_ensino as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_etapa_ensino
			ORDER BY
				cod_etapa_ordem";
	
//	$sql = "SELECT 
//				eteid as codigo,
//				etedesc as descricao
//			FROM 
//				catalogocurso.etapaensino
//			WHERE
//				etestatus = 'A'
//			ORDER BY
//				eteordem ";
	
	combo_popup('cod_etapa_ensino', $sql, 'Etapas de ensino', '400x400', '',
				'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 4, 400 , $onpop = null, $onpush = null, 
				$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
				$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
	
}

function combo_popup_publicoAlvoDemandaSocial($post){
	
	global $db,$padid,$permissoes;
	
	$sql = "SELECT 
				padid as codigo, 
				paddesc as descricao
			FROM 
				catalogocurso.publicoalvodemandasocial
			WHERE
				padstatus = 'A'
			ORDER BY
				2;";
	
	combo_popup('padid', $sql, 'Etapas de ensino', '400x400', '',
				'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 4, 400 , $onpop = null, $onpush = null, 
				$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
				$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
	
}

function monta_combo_nivelEscolaridade( $dados ){
	
	global $db,$permissoes;
	
	$sql = "(SELECT 
				to_char(pk_cod_escolaridade,'9') as codigo, 
				pk_cod_escolaridade||' - '||no_escolaridade as descricao
  			FROM 
  				educacenso_".(ANO_CENSO).".tab_escolaridade)
  			UNION ALL
  			(SELECT 
				to_char(pk_pos_graduacao,'9')||'0' as codigo, 
				pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_pos_graduacao)";
	
	$db->monta_combo('pacod_escolaridade', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', '', '', 'S', 'pacod_escolaridade');
}

function combo_popup_areaFormacao($post){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				pk_cod_area_ocde as codigo, 
				no_nome_area_ocde as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_area_ocde";
	
	combo_popup('cod_area_ocde', $sql, 'Area de Formação', '400x400', '',
				$codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
				$campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null, 
				$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
				$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
	
}

function combo_popup_funcaoExercida($post){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				fexid as codigo, 
				fexdesc as descricao 
			FROM 
				catalogocurso.funcaoexercida
			WHERE
				fexstatus = 'A' ";
	
	combo_popup('fexid', $sql, 'Função Exercida', '400x400', '',
				$codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
				$campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null, 
				$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
				$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
	
}

function combo_popup_disciplina($post){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				pk_cod_disciplina as codigo, 
				no_disciplina as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_disciplina";
	
	combo_popup('cod_disciplina', $sql, 'Disciplina(s) que leciona', $tamanho_janela = '400x400', $maximo_itens = 0,
				$codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
				$campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null, 
				$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
				$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
	
}

function monta_combo_etapaEnsino( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				pk_cod_etapa_ensino as codigo, 
				no_etapa_ensino as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_etapa_ensino
			ORDER BY
				cod_etapa_ordem";
	$db->monta_combo('cod_etapa_ensino', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Etapa de Ensino', '', 'N', 'cod_etapa_ensino'); 
}

function monta_combo_modalidade( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				pk_cod_mod_ensino as codigo, 
				no_mod_ensino as descricao
			FROM 
				educacenso_".(ANO_CENSO).".tab_mod_ensino";
	$db->monta_combo('pacod_mod_ensino', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Modalidade', '', 'S', 'pacod_mod_ensino'); 
}

/*
 * COMPONENTES EQUIPE
 * 
 * */

function monta_combo_categoriaMembroEquipe( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				camid as codigo, 
				camdesc as descricao
			FROM 
				catalogocurso.categoriamembro
			WHERE
				camstatus = 'A'";
	$db->monta_combo('camid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Categoria de membros de equipe.', '', 'S', 'camid'); 
}

function monta_combo_unidadeReferencia( $dados ){
	
	global $db,$permissoes;
	
	$sql = "SELECT 
				unrid as codigo, 
				unrdesc as descricao
			FROM 
				catalogocurso.unidadereferencia
			WHERE
				unrstatus = 'A'";
	$db->monta_combo('unrid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Unidades de Referência', '', 'S', 'unrid'); 	
}

function monta_radio_nivelEscolaridade( $dados ){
	
	global $db,$cod_escolaridade,$permissoes;
	$sql = "SELECT 
				pk_cod_escolaridade as codigo, 
				no_escolaridade as descricao
  			FROM 
  				educacenso_".(ANO_CENSO).".tab_escolaridade";
	$db->monta_radio('cod_escolaridade',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

//Equipes

function listaEquipes( $post, $form = true ){
	
	global $db,$permissoes;
	
	extract($post);
	
	$where = Array('0=0');
	
	if($camid){array_push($where, 'c.camid = '.$camid);}
	if($eqcbolsista){array_push($where, 'eqcbolsista = \''.$eqcbolsista.'\'');}
	if($eqcfuncao){array_push($where, 'eqcfuncao = \''.$eqcfuncao.'\'');}
	if($eqcminimo){array_push($where, 'eqcminimo = '.$eqcminimo);}
	if($eqcmaximo){array_push($where, 'eqcmaximo = '.$eqcmaximo);}
	if($unrid){array_push($where, 'u.unrid = '.$unrid);}
	if($nesid){array_push($where, 'n.nesid = '.$nesid);}
	if($eqcatribuicao){array_push($where, 'eqcatribuicao = \''.$eqcatribuicao.'\'');}
	if($eqcoutrosreq){array_push($where, 'eqcoutrosreq = \''.$eqcoutrosreq.'\'');}
	
	$cabecalho = array("Categoria de Membro<br> de Equipe", "Qtd Função", "Função", "Mínimo", "Máximo", 
					   "Unidade de Referência", "Nivel de Escolaridade", "Atribuição", "Outros Requisitos");
	if( $form ){
		$cabecalho = array("&nbsp;", "&nbsp;", "Categoria de Membro<br> de Equipe", "Qtd Função", "Função", "Mínimo", "Máximo", 
					   "Unidade de Referência", "Nivel de Escolaridade", "Atribuição", "Outros Requisitos");
		$colLista = "'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso.php?modulo=principal/cadEquipe&acao=A&eqcid='|| eqcid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\"></center>' as alterar,
					".($permissoes['gravar']?"'<center><img border=\"0\" align=\"absmiddle\" onclick=\"excluirEquipe('|| eqcid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>' as excluir":" ' - ' ").",";
	}
	
	$sql = "SELECT 
				$colLista
				camdesc,
				qtdfuncao,
				eqcfuncao,
				eqcminimo, 
				eqcmaximo,
				unrdesc,
				eqcid as nesdesc,
				coalesce(replace(eqcatribuicao, chr(13)||chr(10), ''),'o') as artib, 
				coalesce(eqcoutrosreq,'o') as outr
			FROM 
				catalogocurso.equipecurso e
			LEFT JOIN catalogocurso.categoriamembro c ON c.camid = e.camid
			LEFT JOIN catalogocurso.unidadereferencia u ON u.unrid = e.unrid
			LEFT JOIN catalogocurso.nivelescolaridade n ON n.nesid = e.nesid
			WHERE
				eqcstatus = 'A'
				AND e.curid = ".$_SESSION['catalogo']['curid'].
				" AND ".implode(' AND ',$where);
	//ver($_SESSION['catalogo']['curid']); 
	$cursos = $db->carregar($sql);
	if($cursos){
		for($i=0;$i<count($cursos);$i++){
			$eqcid = $cursos[$i]['nesdesc'];
			$sql = "(SELECT 
						pk_cod_escolaridade||' - '||no_escolaridade as descricao
		  			FROM 
		  				educacenso_".(ANO_CENSO).".tab_escolaridade e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
		  			WHERE pk_cod_escolaridade in (select cod_escolaridade from catalogocurso.escolaridade_equipe where eqcid = $eqcid)
		  			order by 1
		  			)
		  			UNION ALL
		  			(SELECT 
						pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao
					FROM 
						educacenso_".(ANO_CENSO).".tab_pos_graduacao e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
		  			WHERE (e.pk_pos_graduacao||'0')::integer in (select cod_escolaridade from catalogocurso.escolaridade_equipe where eqcid = $eqcid)
		  			order by 1
		  			)";
			
			$nivel = $db->carregarColuna($sql);
			if($nivel){
				$cursos[$i]['nesdesc'] = '<div style="width:300px;">'.implode(';<br>',$nivel).'</div>';
			}
			else{
				$cursos[$i]['nesdesc'] = '';
			}
			
		}
	}
	$db->monta_lista_array($cursos, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

// Organização

function monta_combo_tipo(){
	
	global $db,$permissoes,$disabledTioid;
	
	$sql = "SELECT 
				tioid as codigo, 
				tiodesc as descricao 
			FROM 
				catalogocurso.tipoorganizacao
			WHERE
				tiostatus = 'A' ";
	$db->monta_combo('tioid', $sql, ($permissoes['gravar'] && !$disabledTioid ? 'S' : 'N'), 'Selecione...', '', '', 'Tipo de Organização do Curso', '', 'S', 'tioid'); 
}

function listaOrganizacao( $post ){
	
	global $db,$permissoes;
	
	extract($post);
	
	$where = Array('0=0');
	
	if($tioid){array_push($where, 'tor.tioid = '.$tioid);}
	if($orcdesc){array_push($where, "upper(orcdesc) ilike '%upper(removeacento($orcdesc))%'");}
	if($modid){array_push($where, 'mod.modid = \''.$modid.'\'');}
	if($orchapreminimo){array_push($where, 'orchapreminimo = '.$orchapreminimo);}
	if($orchapremaximo){array_push($where, 'orchapremaximo = '.$orchapremaximo);}
	if($orchadisminimo){array_push($where, 'orchadisminimo = '.$orchadisminimo);}
	if($orchadismaximo){array_push($where, 'orchadismaximo = '.$orchadismaximo);}
	if($orcementa){array_push($where, "upper(orcementa) ilike '%upper(removeacento($orcementa))%'");}
	
	$sql = "SELECT 
				'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso.php?modulo=principal/cadOrganizacaoCurso&acao=A&orcid='|| orcid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\"></center>' as alterar,
				".($permissoes['gravar']?"'<center><img border=\"0\" align=\"absmiddle\" onclick=\"excluirOrganizacao('|| orcid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>' as excluir":"' - '").",
				tiodesc, 
				orcdesc, 
				moddesc, 
				coalesce(orcchmim,0) as mim, 
			    coalesce(orcchmax,0) as max,
				coalesce(orcpercpremim,0)||' %' as permim, 
				coalesce(orcpercpremax,0)||' %' as permax, 
				--CASE WHEN LENGTH(orcementa) > 50
				--	THEN SUBSTRING(orcementa FROM 0 FOR 50)||' ...'
				--	ELSE 
					orcementa
				--END
			FROM 
				catalogocurso.organizacaocurso orc
			LEFT JOIN catalogocurso.tipoorganizacao tor ON tor.tioid = orc.tioid
			LEFT JOIN catalogocurso.modalidadecurso mod ON mod.modid = orc.modid
			WHERE
				orcstatus = 'A'
				AND orc.curid = ".$_SESSION['catalogo']['curid'].
				" AND ".implode(' AND ',$where); 
	$cursos = $db->carregar($sql);
	$cabecalho = array("&nbsp;", "&nbsp;", "Tipo", "Nome", "Modalidade", "Hora Aula<br> (Min.)", 
					   "Hora Aula<br> (Máx.)", "Carga Horária Presencial<br>Exigida % (Min.)", "Carga Horária Presencial<br>Exigida % (max.)", "Descrição da Subdvisão");
	$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'S', '100%', '',$arrayDeTiposParaOrdenacao);
}

function montaCabecalho( $curid ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$sql = "SELECT
				curid||' - '||curdesc||' ( versão '||curversao||' )' as dsc
			FROM
				catalogocurso.curso
			WHERE
				curid = $curid";
	
	$dsc = $db->pegaUm($sql);
	
	echo '<script type="text/javascript" src="geral/funcoes.js"></script>';
	echo '<script>
			function criarVersao(){
			 window.location = \'catalogocurso.php?modulo=principal/cadCatalogo&acao=A&curid='.$_SESSION['catalogo']['curid'].'&req=criarversao\';
			}
		  </script>';
	echo '<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr>
				<td class="SubTituloDireita" width="15%">
					<b>Curso(Código):</b>
				</td>
				<td>
					<b>'.$dsc.'</b>
					&nbsp;
					<input type="button" value="Imprimir Curso" onclick="imprimirCurso('.$curid.')"/> 
				</td>
			</tr>
		  </table>';
	
	if( $_SESSION['catalogo']['curid'] ){ 
		$docid = pegaDocidCurso( $_SESSION['catalogo']['curid'] );
		echo '<table align="center" width="95%" border="0" cellpadding="1" cellspacing="1" class="listagem2">
				<tr>
					<td class="SubTituloDireita" width="15%">
						<b>Tramitação:</b>
					</td>
					<td>
						<div>';
		$pflcods    = pegaPerfis($_SESSION['usucpf']);
		if( (!$db->testa_superuser() && !in_array(PERFIL_CONSULTA,$pflcods) && !in_array(PERFIL_ADMINISTRADOR,$pflcods)) && $_SESSION['catalogo']['curid'] != '' ){
			$_REQUEST['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

			$dadosCurso = recuperaDadosCurso($_REQUEST);
			
			$arrCoords = recuperaCoordenacaoResponssavel();
			$arrCurids = recuperaCursoResponssavel();
			
			if( (in_array(PERFIL_GESTOR,$pflcods)&&in_array($dadosCurso['coordid'],$arrCoords)) || (in_array(PERFIL_COORDENADOR,$pflcods)&&in_array($dadosCurso['curid'],$arrCurids) ) ){
			
				$docid = prePegarDocid( $_SESSION['catalogo']['curid'] );
				wf_desenhaBarraNavegacao( $docid , array( 'curid' => $_SESSION['catalogo']['curid']));
			}
		}else{
			if($_SESSION['catalogo']['curid']){
				$docid = prePegarDocid( $_SESSION['catalogo']['curid'] );
				wf_desenhaBarraNavegacao( $docid , array( 'curid' => $_SESSION['catalogo']['curid']));
			}
		}
		echo '</div>
					</td>
				</tr>';
		$esdid = pegaEstadoAtual($docid);
		if( ($db->testa_superuser() || in_array(PERFIL_GESTOR,$pflcods)) && $esdid == WF_EM_VALIDADO_GESTOR ){
		echo   '<tr>
					<td class="SubTituloDireita"><b>Versionamento:</b></td>
					<td>';
			if( possuiVersao() ){
		echo			'<label style="color:red">Este curso possui uma versão mais recente.</label>';	
			}else{
		echo			'<input type="button" value="Criar nova versão" onclick="criarVersao()" />';
			}
		echo		'</td>
				</tr>';
		}
		echo '</table>';
		
	}
}

function possuiVersao(){
	
	global $db;
	
	$sql = "SELECT
				true
			FROM
				catalogocurso.curso
			WHERE
				curidfilho IS NOT NULL AND
				curid = ".$_SESSION['catalogo']['curid'];
	$teste = $db->pegaUm($sql);
	return $teste == 't' ? true : false;
}

?>