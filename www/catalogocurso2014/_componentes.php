<?php
if( !$_SESSION['catalogo']['curid'] ){
    $permissoes['gravar'] = true;
}

// formulário de pesquisa
function monta_checkbox_status_pesquisa($dados){
    global $db,$permissoes;

    $sql = "(SELECT		'A' AS codigo,
						'Ativo' AS descricao)
			 UNION
			(SELECT		'I' AS codigo,
						'Inativo' AS descricao)";
    $marcados = $_POST['curstatus'];
    $db->monta_checkbox('curstatus[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}

function monta_combo_areaTematica_pesquisa($dados){
    global $db,$permissoes;

    $sql = "SELECT 		ateid AS codigo,
						ateid||' - '||atedesc AS descricao
			FROM		catalogocurso2014.areatematica
			WHERE		atestatus = 'A' AND ateano = {$_SESSION['exercicio']}
			ORDER BY	atedesc";
    
    $db->monta_combo('ateid', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Área Temática', '450', 'N', 'ateid', '', $_POST['ateid']);
}

function monta_combo_nivelCurso_pesquisa($dados){
    global $db,$permissoes,$_POST;

    $sql = "SELECT		ncuid AS codigo,
						ncuid||' - '||ncudesc AS descricao
			FROM		catalogocurso2014.nivelcurso
			WHERE		ncustatus = 'A' AND ncuano = {$_SESSION['exercicio']}";
    
    $db->monta_combo('ncuid', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Nivel do Curso', '450', 'N', 'ncuid', '', $_POST['ncuid']);
}

function monta_checkbox_rede_pesquisa($dados){

    global $db,$permissoes;

    $sql = "SELECT 		redid as codigo,
						reddesc as descricao
			FROM		catalogocurso2014.rede
			WHERE		redstatus = 'A' AND redano = {$_SESSION['exercicio']}";
    
    $marcados = $_POST['redid'];
    $db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}

//formulários de cadastro

function monta_checkbox_rede($dados){
    global $db,$permissoes;

    $sql = "SELECT 		redid AS codigo,
						reddesc AS descricao
			FROM		catalogocurso2014.rede
			WHERE		redstatus = 'A' AND redano = {$_SESSION['exercicio']}";
    
    $marcados = $dados['redid'];
    $db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}

function monta_checkbox_status($dados){
    global $db,$permissoes;

    $sql = "(SELECT		'A' AS codigo,
						'Ativo' AS descricao)
			 UNION
			(SELECT		'I' AS codigo,
						'Inativo' AS descricao)";
    $marcados = $_POST['redid'];
    $db->monta_checkbox('redid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}

function monta_checkbox_modalidade($dados){
    global $db,$permissoes;

    $sql = "SELECT 		pk_cod_mod_ensino AS codigo,
						no_mod_ensino AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_mod_ensino";

    $marcados = $dados['cod_mod_ensino'];
    $db->monta_checkbox('cod_mod_ensino[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}

function monta_checkboxcheckbox_modalidadeCurso($dados){
    global $db,$permissoes;

    $sql = "SELECT 		modid as codigo,
						moddesc as descricao
			FROM		catalogocurso2014.modalidadecurso
			WHERE		modstatus = 'A' AND modano = {$_SESSION['exercicio']}";

    $marcados = $dados['modid'];
    $db->monta_checkbox('modid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));

}

function monta_combo_areaTematica($dados){
    global $db,$permissoes;

    $sql = "SELECT		ateid AS codigo,
						ateid||' - '||atedesc AS descricao
			FROM		catalogocurso2014.areatematica 
			WHERE		atestatus = 'A' AND ateano = {$_SESSION['exercicio']}
			ORDER BY	atedesc";
    $db->monta_combo('ateid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Área Temática', '450', 'S', 'ateid');
}

function monta_combo_nivelCurso($dados){
    global $db,$permissoes;

    $sql = "SELECT		ncuid AS codigo,
						ncuid||' - '||ncudesc AS descricao
			FROM		catalogocurso2014.nivelcurso 
			WHERE		ncustatus = 'A' AND ncuano = {$_SESSION['exercicio']}";
    
    $db->monta_combo('ncuid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Nivel do Curso', '450', 'S', 'ncuid');
}

function monta_checkbox_nivelCurso($dados){
    global $db,$permissoes;

    $sql = "SELECT 		ncuid AS codigo,
						ncudesc AS descricao
			FROM		catalogocurso2014.nivelcurso
			WHERE		ncustatus = 'A' AND ncuano = {$_SESSION['exercicio']}";
    
    $marcados = $dados['ncuid'];
    $db->monta_checkbox('ncuid[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar']));
}


function monta_combo_localizacaoEscola($dados){
    global $db,$permissoes;

    $sql = "SELECT		lesid AS codigo,
						lesdesc AS descricao
			FROM		catalogocurso2014.localizacaoescola
			WHERE		lesstatus = 'A' AND lesano = {$_SESSION['exercicio']}";
    
    $db->monta_combo('lesid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Localização da Escola', '450', 'S', 'lesid');
}

function monta_combo_localizacaoDiferenciadaEscola($dados){
    global $db,$permissoes;

    $sql = "SELECT		ldeid AS codigo,
						ldedesc AS descricao
			FROM		catalogocurso2014.localizacaodiferenciadaescola
			WHERE		ldestatus = 'A' AND ldeano = {$_SESSION['exercicio']}";
    $db->monta_combo('ldeid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Localização Diferenciada da Escola', '450', 'S', 'ldeid');
}

function monta_combo_modalidadeCurso($dados){
    global $db,$permissoes;

    $sql = "SELECT		modid AS codigo,
						moddesc AS descricao
			FROM		catalogocurso2014.modalidadecurso
			WHERE		modstatus = 'A' AND modano = {$_SESSION['exercicio']}";
    
    $db->monta_combo('modid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Modalidade do Curso', '450', 'S', 'modid');
}

function monta_radio_uniMedMonitora($dados){
    global $db, $permissoes;

    $sql = "SELECT 		uteid AS codigo,
						utedesc AS descricao
			FROM		catalogocurso2014.unidadetempo
			WHERE		utestatus = 'A' AND uteano = {$_SESSION['exercicio']}
            ORDER BY 	uteid";
    
    $uteid = $uteid ? $uteid : 1;
    $db->monta_radio('uteid',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_multi($dados){
    global $db,$permissoes;

    $sql = "(SELECT		't' AS codigo,
						'Sim' AS descricao)
			 UNION
			(SELECT		'f' AS codigo,
						'Não' AS descricao)
			ORDER BY	2 DESC";
    $cursalamulti = $cursalamulti ? $cursalamulti : 'f';
    $db->monta_radio('cursalamulti',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_status($dados){
    global $db,$curstatus,$permissoes;

    $sql = "(SELECT		'A' AS codigo,
						'Ativo' AS descricao)
			 UNION
			(SELECT		'I' AS codigo,
						'Inativo' AS descricao)";
    $curstatus = $curstatus ? $curstatus : 'A';
    $db->monta_radio('curstatus',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op,Array("obrigatorio"=>true));
}

function monta_radio_bolsistas($dados){
    global $db,$eqcbolsista,$permissoes;

    $sql = "(SELECT		't' AS codigo,
						'Sim' AS descricao)
			 UNION
			(SELECT
						'f' AS codigo,
						'Não' AS descricao)
			ORDER BY	2 DESC";
    $eqcbolsista = $eqcbolsista ? $eqcbolsista : 'f';
    $db->monta_radio('eqcbolsista',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_radio_ofertaNacional($dados){
    global $db,$curofertanacional,$permissoes;

    $sql = "(SELECT		'true' AS codigo,
						'Sim' AS descricao)
			 UNION
			(SELECT  	'false' AS codigo,
						'Não' AS descricao)
			ORDER BY	2 DESC";
    $curofertanacional = $curofertanacional == 't' ? 'true' : 'false';
    $db->monta_radio('curofertanacional',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

function monta_combo_resp($dados){
    global $db,$permissoes;

    $aryWhere[] = "coordstatus = 'A'";
    
    $aryWhere[] = "coorano = '{$_SESSION['exercicio']}'";
    
    if(!$db->testa_superuser()){
        $arrCoords = recuperaCoordenacaoResponssavel();
    }

    if(is_array($arrCoords) && count($arrCoords)>0){
  		$aryWhere[] = "coordid IN (".implode(",",$arrCoords).")";  	
    }
    
    $sql = "SELECT		coordid AS codigo,
						coordsigla||' - '||coorddesc AS descricao
			FROM		catalogocurso2014.coordenacao
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') ."
			ORDER BY	coordsigla";
    
    if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){
    	$db->monta_combo('coordid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '450', 'S', 'coordid');
    } else {
    	$db->monta_combo('coordid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', 'atualizarCoordenacaoResponsavel', '', 'Diretoria Responsável', '450', 'S', 'coordid');
    }
}

function listaCursos( $request ){
    global $db;

    $arrPflcods = Array();
    $arrCurids  = Array();
    $arrCoords  = Array();
    $pflcods  = pegaPerfis($_SESSION['usucpf']);
    
    $aryWhere[] = "c.curstatus = 'A' AND c.curano = {$_SESSION['exercicio']}";

    if(!$db->testa_superuser() || in_array(PERFIL_CONSULTA,$pflcods) || in_array(PERFIL_ADMINISTRADOR,$pflcods)){
        $arrCoords = recuperaCoordenacaoResponssavel();
        $arrCurids = recuperaCursoResponssavel();
        
        if(is_array($arrCoords)){
        	array_push($arrCoords,'0');
        }
        
        if(is_array($arrCurids)){
        	array_push($arrCurids,'0');
        }
                
        $pflcod     = pegaPerfil($_SESSION['usucpf']);
        $arrPflcods = Array(PERFIL_COORDENADOR,PERFIL_GESTOR);
    }

    if($request['curstatus']){
    	$aryWhere[] = "curstatus IN ('".implode("','",$request['curstatus'])."')";
    }

    if($request['ateid']){
    	$aryWhere[] = "a.ateid = {$request['ateid']}";
    }

    if($request['suaid'] && is_numeric($request['suaid'])){
    	$aryWhere[] = "s.suaid = {$request['suaid']}";
    }

    if($request['espid'] && is_numeric($request['espid'])){
    	$aryWhere[] = "e.espid = {$request['espid']}";
    }

    if($request['curid']){
    	$aryWhere[] = "c.curid = {$request['curid']}";
    }

    if($request['curdesc']){
    	$aryWhere[] = "curdesc ILIKE '%".$request['curdesc']."%'";
    }

    if($request['ncuid']){
    	$aryWhere[] = "nc.ncuid = {$request['ncuid']}";
    }

    if($request['coordid']){
    	$aryWhere[] = "c.coordid = {$request['coordid']}";
    }

    if($request['redid']){
    	$aryWhere[] = "redid IN (".implode(",",$request['redid']).")";
    }

    if($request['dt1']){
        $request['dt1'] = explode("/",$request['dt1']);
        $request['dt1'] = $request['dt1'][2]."/".$request['dt1'][1]."/".$request['dt1'][0];
        $aryWhere[] = "curdtinclusao >= '{$request['dt1']}'";
    }

    if($request['dt2']){
    	$aryWhere[] = "plaid = {$_SESSION['plaid']}";
        $request['dt2'] = explode("/",$request['dt2']);
        $request['dt2'] = $request['dt2'][2]."/".$request['dt2'][1]."/".$request['dt2'][0];
        $aryWhere[] = "curdtinclusao >= '{$request['dt2']}'";
    }
	
    if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){
    	$nivelcurso = "ncudesc AS nivelcurso,";
    	
    } else {
    	$nivelcurso = "array_to_string(array(
    				  		SELECT 		nc.ncudesc 
							FROM		catalogocurso2014.nivelcurso nc
							INNER JOIN   catalogocurso2014.nivelcurso_curso nl ON nc.ncuid = nl.ncuid 
							WHERE 		curid = c.curid
					   ), ',') AS nivelcurso,";
    	
    }
    
    $sql = "SELECT 		DISTINCT	'<center><img border=\"0\" align=\"absmiddle\" onclick=\"imprimirCurso('||c.curid||')\" title=\"Imprimir\" style=\"cursor: pointer\" src=\"../imagens/print.gif\"></center>' as imprimir,
									'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A&curid='|| c.curid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\"></center>' as alterar,
									".( $db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR,$pflcods) ? "'<center><img border=\"0\" align=\"absmiddle\" onclick=\"excluirCurso('|| c.curid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>'" : "' - '" )."
				 					AS excluir,
						atedesc AS areatematica,
						suadesc,
						espdesc,
						c.curid,
						c.curdesc AS curso,
						$nivelcurso
						esd.esddsc
			FROM		catalogocurso2014.curso c
			INNER JOIN 	workflow.documento doc ON doc.docid = c.docid
			INNER JOIN 	workflow.estadodocumento esd ON esd.esdid = doc.esdid
			LEFT JOIN 	catalogocurso2014.especialidade e ON e.espid = c.espid
			LEFT JOIN 	catalogocurso2014.subarea s ON s.suaid = c.suaid
			LEFT JOIN 	catalogocurso2014.areatematica a ON a.ateid = c.ateid
			INNER JOIN 	catalogocurso2014.cursorede r ON r.curid = c.curid
			LEFT JOIN 	catalogocurso2014.nivelcurso nc ON nc.ncuid = c.ncuid
			LEFT JOIN 	catalogocurso2014.usuarioresponsabilidade ur1 ON ur1.coordid = c.coordid
			LEFT JOIN 	catalogocurso2014.usuarioresponsabilidade ur2 ON ur2.curid = c.curid
						".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."";
    
    $cursos = $db->carregar($sql, null);
    $cabecalho = array("&nbsp;", "&nbsp;", "&nbsp;", "Área", "Subárea", "Especialidade", "Código Curso", "Nome do Curso", "Nivel Curso", "Situação");
    $db->monta_lista_array($cursos, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

function combo_popup_etapaEnsino($post){
    global $db,$cod_etapa_ensino,$permissoes;

    $sql = "SELECT		pk_cod_etapa_ensino::integer AS codigo,
						replace(no_etapa_ensino,'-','/') AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_etapa_ensino
			ORDER BY 	codigo ASC";

    combo_popup('cod_etapa_ensino', $sql, 'Todas', '400x400', '',
    	'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 4, 400 , $onpop = null, $onpush = null,
        $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
        $funcaoJS=null, $intervalo=false, $arrVisivel, array('cod'));
}

function combo_popup_publicoAlvoDemandaSocial($post){
    global $db,$padid,$permissoes;

    $sql = "SELECT		padid AS codigo,
						paddesc AS descricao
			FROM		catalogocurso2014.publicoalvodemandasocial
			WHERE		padstatus = 'A' AND padano = {$_SESSION['exercicio']}
			ORDER BY	2";

    combo_popup('padid', $sql, 'Etapas de ensino', '400x400', '',
        '', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 4, 400 , $onpop = null, $onpush = null,
        $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
        $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);

}

function monta_combo_nivelEscolaridade($dados){
    global $db,$permissoes;

    $sql = "(SELECT		to_char(pk_cod_escolaridade,'9') as codigo,
						pk_cod_escolaridade||' - '||no_escolaridade as descricao
  			 FROM		educacenso_".(ANO_CENSO).".tab_escolaridade)
  			 UNION ALL
  			(SELECT		to_char(pk_pos_graduacao,'9')||'0' as codigo,
						pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao
			 FROM		educacenso_".(ANO_CENSO).".tab_pos_graduacao)";

    $db->monta_combo('pacod_escolaridade', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', '', '', 'S', 'pacod_escolaridade');
}

function combo_popup_areaFormacao($post){
    global $db,$permissoes;

    $sql = "SELECT		pk_cod_area_ocde AS codigo,
						no_nome_area_ocde AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_area_ocde";

    combo_popup('cod_area_ocde', $sql, 'Teste', '400x400', '',
        $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
        $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
        $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
        $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
}

function combo_popup_funcaoExercida($post){
    global $db,$permissoes;

    $sql = "SELECT		fexid AS codigo,
						fexdesc AS descricao
			FROM		catalogocurso2014.funcaoexercida
			WHERE		fexstatus = 'A' AND fexano = {$_SESSION['exercicio']}";

    combo_popup('fexid', $sql, 'Função Exercida', '400x400', '',
        $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
        $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
        $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
        $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);

}

function combo_popup_disciplina($post){
    global $db,$permissoes;

    $sql = "SELECT		pk_cod_disciplina AS codigo,
						no_disciplina AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_disciplina";

    combo_popup('cod_disciplina', $sql, 'Todos', $tamanho_janela = '400x400', $maximo_itens = 0,
        $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
        $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
        $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
        $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
}

function monta_combo_etapaEnsino($dados){
    global $db,$permissoes;

    $sql = "SELECT		pk_cod_etapa_ensino AS codigo,
						no_etapa_ensino AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_etapa_ensino
			ORDER BY	cod_etapa_ordem";
    $db->monta_combo('cod_etapa_ensino', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Etapa de Ensino', '', 'N', 'cod_etapa_ensino');
}

function monta_combo_modalidade($dados){
    global $db,$permissoes;

    $sql = "SELECT		pk_cod_mod_ensino AS codigo,
						no_mod_ensino AS descricao
			FROM		educacenso_".(ANO_CENSO).".tab_mod_ensino";
    
    $db->monta_combo('pacod_mod_ensino', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Modalidade', '', 'S', 'pacod_mod_ensino');
}

/*
 * COMPONENTES EQUIPE
 *
 * */
function monta_combo_categoriaMembroEquipe($dados){
    global $db,$permissoes;

    $sql = "SELECT 		camid AS codigo,
						camdesc AS descricao
			FROM		catalogocurso2014.categoriamembro
			WHERE		camstatus = 'A' AND camano = {$_SESSION['exercicio']}
			ORDER BY 	camdesc";
    
    $db->monta_combo('camid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Categoria de membros de equipe.', '', 'S', 'camid');
}

function monta_combo_unidadeReferencia($dados){
    global $db,$permissoes;

    $sql = "SELECT 		unrid AS codigo,
						unrdesc AS descricao
			FROM		catalogocurso2014.unidadereferencia
			WHERE		unrstatus = 'A' AND unrano = {$_SESSION['exercicio']}
			ORDER BY	unrdesc";
    
    $db->monta_combo('unrid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Unidades de Referência', '', 'N', 'unrid');
}

function monta_combo_Funcao($dados){
    global $db,$permissoes;

    $sql = "SELECT 		fueid AS codigo,
						fuedesc || 
						CASE WHEN fuesecretaria = '1' THEN ' - SEB'
							 WHEN fuesecretaria = '2' THEN ' - SECADI' 
							 ELSE '' END AS descricao
			FROM		catalogocurso2014.funcaoequipe
			WHERE		fuestatus = 'A' AND fueano = {$_SESSION['exercicio']}
			ORDER BY	descricao";
    
    $db->monta_combo('fueid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', 'carregarEquipeAtribuicaoEscolaridade', '', 'Função', '', 'N', 'fueid');
}

function monta_radio_nivelEscolaridade($dados){
    global $db,$cod_escolaridade,$permissoes;

    $sql = "SELECT	pk_cod_escolaridade AS codigo,
					no_escolaridade AS descricao
  			FROM	educacenso_".(ANO_CENSO).".tab_escolaridade";
    
    $db->monta_radio('cod_escolaridade',$sql,($permissoes['gravar'] ? 'S' : 'N'),$op);
}

//Equipes
function listaEquipes( $post, $form = true ){
    global $db,$permissoes;

    extract($post);

    $aryWhere[] = "eqcstatus = 'A'";
    
    $aryWhere[] = "e.curid = {$_SESSION['catalogo']['curid']}";
    
    if($camid){
    	$aryWhere[] = "c.camid = {$camid}";
    }
    
    if($eqcbolsista){
    	$aryWhere[] = "eqcbolsista = '{$eqcbolsista}'";
    }
    
    if($eqcfuncao){
    	$aryWhere[] = "eqcfuncao = '{$eqcfuncao}'";
    }
    
    if($eqcminimo){
    	$aryWhere[] = "eqcminimo = {$eqcminimo}";
    }
    
    if($eqcmaximo){
    	$aryWhere[] = "eqcmaximo = {$eqcmaximo}";
    }
    
    if($unrid){
    	$aryWhere[] = "u.unrid = {$unrid}";
    }
    
    if($nesid){
    	$aryWhere[] = "n.nesid = {$nesid}";
    }
    
    if($eqcatribuicao){
    	$aryWhere[] = "eqcatribuicao = '{$eqcatribuicao}'";
    }
    
    if($eqcoutrosreq){
    	$aryWhere[] = "eqcoutrosreq = '{$eqcoutrosreq}'";
    }

    $cabecalho = array("Categoria de Membro<br> de Equipe", "Qtd Função", "Função", "Mínimo", "Máximo",
        "Unidade de Referência", "Nivel de Escolaridade", "Atribuição", "Outros Requisitos");
    if( $form ){
        $cabecalho = array("&nbsp;", "&nbsp;", "Categoria de Membro<br> de Equipe", "Qtd Função", "Função", "Mínimo", "Máximo",
            "Unidade de Referência", "Nivel de Escolaridade", "Atribuição", "Outros Requisitos");
        $colLista = "'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A&eqcid='|| eqcid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\"></center>' as alterar,
					".($permissoes['gravar']?"'<center><img border=\"0\" align=\"absmiddle\" onclick=\"excluirEquipe('|| eqcid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>' as excluir":" ' - ' ").",";
    }

    $sql = "SELECT			$colLista
							camdesc,
							qtdfuncao,
							CASE WHEN e.fueid IS NULL THEN e.eqcfuncao
								 ELSE f.fuedesc END AS eqcfuncao,
							eqcminimo,
							eqcmaximo,
							unrdesc,
							eqcid AS nesdesc,
							coalesce(replace(eqcatribuicao, chr(13)||chr(10), ''),'o') AS artib,
							coalesce(eqcoutrosreq,'o') AS outr
			FROM			catalogocurso2014.equipecurso e
			LEFT JOIN 		catalogocurso2014.categoriamembro c ON c.camid = e.camid
			LEFT JOIN 		catalogocurso2014.unidadereferencia u ON u.unrid = e.unrid
			LEFT JOIN 		catalogocurso2014.nivelescolaridade n ON n.nesid = e.nesid
			LEFT JOIN 		catalogocurso2014.funcaoequipe f ON e.fueid = f.fueid	
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') ."";

    $cursos = $db->carregar($sql);
    if($cursos){
        for($i=0;$i<count($cursos);$i++){
            $eqcid = $cursos[$i]['nesdesc'];
            $sql = "(SELECT			pk_cod_escolaridade||' - '||no_escolaridade AS descricao
		  			 FROM			educacenso_".(ANO_CENSO).".tab_escolaridade e
		  			 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
		  			 WHERE 		    h.hneano = {$_SESSION['exercicio']} AND pk_cod_escolaridade IN (SELECT cod_escolaridade FROM catalogocurso2014.escolaridade_equipe WHERE eqcid = {$eqcid} AND eeqano = {$_SESSION['exercicio']})
		  			 ORDER BY 		1)
		  			 UNION ALL
		  			(SELECT			pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao
					 FROM			educacenso_".(ANO_CENSO).".tab_pos_graduacao e
		  			 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
		  			 WHERE 			h.hneano = {$_SESSION['exercicio']} AND (e.pk_pos_graduacao||'0')::integer IN (SELECT cod_escolaridade FROM catalogocurso2014.escolaridade_equipe WHERE eqcid = {$eqcid} AND eeqano = {$_SESSION['exercicio']})
		  			 ORDER BY 		1)";

            $nivel = $db->carregarColuna($sql);
            if($nivel){
                $cursos[$i]['nesdesc'] = '<div style="width:300px;">'.implode(';<br>',$nivel).'</div>';
            } else {
                $cursos[$i]['nesdesc'] = '';
            }

        }
    }
    $db->monta_lista_array($cursos, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

// Organização

function monta_combo_tipo(){
    global $db,$permissoes,$disabledTioid;

    $sql = "SELECT		tioid AS codigo,
						tiodesc AS descricao
			FROM		catalogocurso2014.tipoorganizacao
			WHERE		tiostatus = 'A' AND tioano = {$_SESSION['exercicio']}";
    
    $db->monta_combo('tioid', $sql, ($permissoes['gravar'] && !$disabledTioid ? 'S' : 'N'), 'Selecione...', '', '', 'Tipo de Organização do Curso', '200', 'S', '');
}

function listaOrganizacao( $post ){
    global $db,$permissoes;

    extract($post);

    if($tioid){
    	$aryWhere[] = "tor.tioid = {$tioid}";
    }
    
    if($orcdesc){
    	$aryWhere[] = "upper(orcdesc) ILIKE '%upper(removeacento({$orcdesc}))%'";
    }
    
    if($modid){
    	$aryWhere[] = "mod.modid = '{$modid}'";
    }
    
    if($orchapreminimo){
    	$aryWhere[] = "orchapreminimo = {$orchapreminimo}";
    }
    
    if($orchapremaximo){
    	$aryWhere[] = "orchapremaximo = {$orchapremaximo}";
    }
    
    if($orchadisminimo){
    	$aryWhere[] = "orchadisminimo = {$orchadisminimo}";
    }
    
    if($orchadismaximo){
    	$aryWhere[] = "orchadismaximo = {$orchadismaximo}";
    }
    
    if($orcementa){
    	$aryWhere[] = "upper(orcementa) ILIKE '%upper(removeacento({$orcementa}))%'";
    }
    
    $aryWhere[] = "orcstatus = 'A'";
    
    $aryWhere[] = "orc.curid = {$_SESSION['catalogo']['curid']}";

    $sql = "SELECT			'<center><img border=\"0\" align=\"absmiddle\" onclick=\"window.location=\\'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A&orcid='|| orcid ||'\\'\" title=\"Alterar\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\">' as alterar,
							".($permissoes['gravar']?"'<img border=\"0\" align=\"absmiddle\" onclick=\"excluirOrganizacao('|| orcid ||');\" title=\"Excluir.\" style=\"cursor:pointer\" src=\"/imagens/excluir.gif\"></center>' as excluir":"' - '").",
							tiodesc,
							orcdesc,
							coalesce(orcchmim,0) AS mim,
							orcementa
			FROM			catalogocurso2014.organizacaocurso orc
			LEFT JOIN 		catalogocurso2014.tipoorganizacao tor ON tor.tioid = orc.tioid
			LEFT JOIN 		catalogocurso2014.modalidadecurso mod ON mod.modid = orc.modid
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') ."";

    $cursos = $db->carregar($sql);
    $cabecalho = array("Ação", "Tipo", "Nome", "Carga Horária", "Descrição");

    if($cursos){
        foreach($cursos as &$curso){
            array_unshift($curso , $curso['alterar'] . ' &nbsp; ' .  $curso['excluir']);
            unset($curso['alterar']);
            unset($curso['excluir']);
        }
    }
    
    $db->monta_lista_array($cursos, $cabecalho, 50, 20, 'INT', '100%', '',$arrayDeTiposParaOrdenacao);
}

function montaCabecalho( $curid ){
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    $sql = "SELECT		curid||' - '||curdesc||' ( versão '||curversao||' )' AS dsc
			FROM		catalogocurso2014.curso
			WHERE		curid = {$curid} AND curano = {$_SESSION['exercicio']}";

    $dsc = $db->pegaUm($sql);

    echo '<script type="text/javascript" src="geral/funcoes.js"></script>';
    echo '<script>
			function criarVersao(){
			 window.location = \'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A&curid='.$_SESSION['catalogo']['curid'].'&req=criarversao\';
			}
		  </script>';
    echo '<table align="center" width="98%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr>
				<td class="SubTituloDireita" width="15%">
					<b>Curso(Código):</b>
				</td>
				<td>
					<b>'.$dsc.'</b>
					&nbsp;

				</td>
			</tr>
		  </table>';

    if( $_SESSION['catalogo']['curid'] ){
        $docid = pegaDocidCurso( $_SESSION['catalogo']['curid'] );
        echo '<table align="center" width="98%" border="0" cellpadding="1" cellspacing="1" class="listagem2">
				<tr>
					<td class="SubTituloDireita" width="15%">
						<b>Tramitação:</b>
					</td>
					<td>
						<div>';
        $pflcods = pegaPerfis($_SESSION['usucpf']);

        wf_desenhaBarraNavegacao( $docid , array( 'curid' => $_SESSION['catalogo']['curid']));

        echo '</div>
					</td>
				</tr>';
        $esdid = pegaEstadoAtual($docid);
        if( ($db->testa_superuser() || in_array(PERFIL_GESTOR,$pflcods)) && $esdid == WF_EM_VALIDADO_GESTOR ){
            echo   '<tr>
					<td class="SubTituloDireita"><b>Versionamento:</b></td>
					<td>';
            echo		'</td>
				</tr>';
        }
        echo '</table>';
    }
}

function possuiVersao(){
    global $db;

    $sql = "SELECT		true
			FROM		catalogocurso2014.curso
			WHERE		curidfilho IS NOT NULL AND curid = {$_SESSION['catalogo']['curid']}";
    
    $teste = $db->pegaUm($sql);
    return $teste == 't' ? true : false;
}

?>