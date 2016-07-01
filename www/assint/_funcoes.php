<?PHP

    function exibirListaAtividades( $data ){
        global $db;
        
        if( $data['pecid'] > 0 ){
            $where = "WHERE atvaba = 'P' AND pecid = {$data['pecid']};";
        }elseif( $data['ofpid'] > 0 ){
            $where = "WHERE atvaba = 'V' AND ofpid = {$data['ofpid']};";           
        }

        $sql = "
            SELECT  u.usunome,
                    atvdsc_dados_ant,
                    atvdsc_dados_alt,
                    to_char(atvdtinclusao, 'DD/MM/YYYY') as dt_inclusao
            FROM assint.atividadehistorico AS a
            JOIN seguranca.usuario  AS u ON u.usucpf = a.usucpf
            $where
        ";
        $dados = $db-> carregar($sql);

        foreach( $dados as $_dados){
            $atvdsc_dados_ant = explode(';', $_dados['atvdsc_dados_ant']);
            $atvdsc_dados_alt = explode(';', $_dados['atvdsc_dados_alt']);
?>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0" width="99%">
                <tr>
                    <td colspan="4" class="subTituloCentro"> Listagem das Alterações executadas </td>
                </tr>
                <tr>
                    <td class="subTituloEsquerda" colspan="4"> A operação foi realizada na data de: <span style="color: red;"> <?=$_dados['dt_inclusao']?> </span> pelo usuário: <span style="color: red;"><?=$_dados['usunome']?> </span> </td>
                </tr>
                <tr>
                    <td class="subTituloCentro" colspan="2" width="45%"> Dados Originais </td>
                    <td class="subTituloCentro" colspan="2" width="45%"> Dados Alterados </td>
                </tr>
                    <?PHP
                        $controle = 0;
                        foreach( $atvdsc_dados_ant as $atvdsc_ant ){
                            #COLUNAS COM OS DADOS ORIGINAIS.
                            $col_ant_dsc = substr( $atvdsc_ant, 0, strpos($atvdsc_ant, ":") );
                            $col_ant_val = substr( $atvdsc_ant, strpos($atvdsc_ant, ":")+1  );

                            #COLUNAS COM OS DADOS ALTERADOS.
                            $col_alt_dsc = substr( $atvdsc_dados_alt[$controle], 0, strpos($atvdsc_dados_alt[$controle], ":") );
                            $col_alt_val = substr( $atvdsc_dados_alt[$controle], strpos($atvdsc_dados_alt[$controle], ":")+1  );
                            
                            if( strtoupper(trim($col_ant_val)) != strtoupper(trim($col_alt_val)) ){
                                $style = 'style="background-color: #ffffcc !important; font-style: oblique;"';
                            }else{
                                $style = '';
                            }
                    ?>
                            <tr <?=$style;?>>
                            <!--<tr style="background-color: #ffffcc !important;">-->
                                <!-- COLUNAS COM OS DADOS ORIGINAIS -->
                                <td class="subTituloDireita" style="text-transform:uppercase;"><?=$col_ant_dsc;?>:</td>
                                <td style="text-transform:capitalize;"> <?=$col_ant_val;?> </td>

                                <!-- COLUNAS COM OS DADOS ALTERADOS -->
                                <?PHP
                                    if( count($atvdsc_dados_alt[1]) > 0 ){
                                ?>
                                        <td class="subTituloDireita" style="text-transform:uppercase;"><?=$col_alt_dsc;?>:</td>
                                        <td style="text-transform:capitalize;"><?=$col_alt_val;?></td>
                                <?PHP
                                    }else{
                                ?>
                                        <td> Operação de Deleção, Não há dados a serem exibidos. </td>
                                <?  } ?>
                            </tr>
                    <?PHP
                            $controle = $controle + 1;
                        }
                    ?>
            </table>
<?PHP
        }
        die();
    }

/**
 * Redireciona o navegador para a tela indicada.
 *
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() ) {

	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

function redir($url = null, $msg=null){
	$script .= '<script>';
	if (!empty($msg))
		$script .= '	alert(\'' . $msg . '\');';

	if (!empty($url))
		$script .= '	location.href=\'' . $url . '\';';
	else
		$script .= '	history.go(-1);';

	$script .= '</script>';
	die($script);
}

/**
 * @Recupera os perfis do usuário
 *
 * @return array
 *
 */
function recuperaPerfil() {
	global $db;

	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = ".SISID_ASSINT."
			WHERE
				pu.usucpf = '".$_SESSION["usucpf"]."'";

	return $db->carregarColuna($sql);
}

/**
 * @Recupera as universidades(entidades) associadas ao usuário pelo perfil 'Universidades'
 *
 * @return mixed
 *
 */
function recuperaUniversidades() {
	global $db;

	$sql = "SELECT
				entid
			FROM
				assint.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION["usucpf"]."'
				AND pflcod = ".PERFIL_UNIVERSIDADES."
				AND rpustatus = 'A'";
	$entids = $db->carregarColuna($sql);

	return ($entids) ? $entids : false;
}

    function excluirArquivo($dados){
	global $db;

	$sql = "UPDATE assint.anexos SET anxstatus = 'I' WHERE anxid = {$dados['anxid']} RETURNING anxid;";
        $anxid = $db->pegaUm( $sql );

	if( $anxid > 0 ){
            $db->commit();
            //return true;
            echo 'S';
	}else{
            $db->rollback();
            //return false;
            echo 'N';
	}
    }

    function cadastrarCurso($post){
        global $db;
        
        extract($post);
        
        $co_ies = $post['co_ies'] == '' ? $post['co_ies'][0] : $post['co_ies'];

        if( $co_ies > 0 ){
            $ofpobservacao = substituir_char_especiais_word( $ofpobservacao );
            $ofpiescampus = substituir_char_especiais_word( $ofpiescampus );

            $ofpobservacao = trim( addslashes( $ofpobservacao ) );
            $ofpiescampus  = trim( addslashes( $ofpiescampus ) );

            $sql = "
                INSERT INTO assint.ofertacursopecg(
                    co_curso, ofpnumvagas, habid, co_ies, usucpf, ofpturno, ofpanosemestre, ofpobservacao, ofpano, ofpiescampus
                ) VALUES (
                    {$co_curso}, {$ofpnumvagas}, {$habid}, {$co_ies}, '{$_SESSION['usucpf']}', '{$ofpturno}', '{$ofpanosemestre}', '{$ofpobservacao}', {$ofpano}, '{$ofpiescampus}'
                ) RETURNING ofpid;
            ";
            $ofpid = $db->pegaUm($sql);

            if( $ofpid > 0 ){
                $db->commit();
                $db->sucesso( 'principal/PEC_G','&aba=vagasPEC_G&ano='.$ofpano.'&ofpid='.$ofpid, 'Dados gravados com sucesso!', 'N', 'S' );
            }else{
                $db->rollback();
                $db->sucesso( 'principal/cadCursoPEC_G','', 'Operação não realizada, tente novamente mais tarde!' );
            }
        } else {
            $db->sucesso( 'principal/cadCursoPEC_G','', 'Operação não realizada, tente novamente mais tarde!' );
        }
    }
    
    function alterarCurso($post){
	global $db;
        
	extract($post);
        
        $back = backUpAtividadesVagas( $post, 'A' );

        $co_ies = $post['co_ies'] == '' ? $post['co_ies'][0] : $post['co_ies'];      
        
        $ofpobservacao = substituir_char_especiais_word( $ofpobservacao );
        $ofpiescampus = substituir_char_especiais_word( $ofpiescampus );
        
        $ofpobservacao = trim( addslashes( $ofpobservacao ) );
        $ofpiescampus  = trim( addslashes( $ofpiescampus ) );
        
	$sql = "
            UPDATE assint.ofertacursopecg
                SET co_curso        = {$co_curso},
                    co_ies          = {$co_ies},
                    ofpnumvagas     = {$ofpnumvagas},
                    habid           = '{$habid}',
                    ofpturno        = '{$ofpturno}',
                    ofpanosemestre  = '{$ofpanosemestre}',
                    ofpobservacao   = '{$ofpobservacao}',
                    ofpano          = {$ofpano},
                    ofpiescampus    = '{$ofpiescampus}'
            WHERE ofpid = {$ofpid} RETURNING ofpid;
        ";
        $ofpid = $db->pegaUm($sql);

        if( $ofpid > 0 && $back == true ){
            $db->commit();
            $db->sucesso( 'principal/PEC_G','&aba=vagasPEC_G&ano='.$ofpano.'&ofpid='.$ofpid, 'Dados Alterados com sucesso!', 'N', 'S' );
        }else{
            $db->rollback();
            $db->sucesso( 'principal/cadCursoPEC_G','', 'Operação não realizada, tente novamente mais tarde!' );
        }
    }

    function excluirCurso($post){
	global $db;

	extract($post);
        
        $back = backUpAtividadesVagas( $post, 'D' );

	if( $ofpid != '' ){
            $sql = "UPDATE assint.ofertacursopecg SET ofpstatus = 'I' WHERE ofpid = {$ofpid} RETURNING ofpid";
	}
        $_ofpid = $db->pegaUm($sql);
        
        if( $_ofpid > 0 && $back == true ){
            $db->commit();
	}
    }

    function backUpAtividadesVagas( $dados = NULL, $tipo ){
        global $db;

        #OPERAÇÃO DE DELEÇÃO
        if( $tipo == 'D' ){
            $ofpid              = $dados['ofpid'];
            $usucpf             = $_SESSION['usucpf'];
            $atvdsc_dados_alt   = 'Operação de deleção, não dados!';

            $sql = "
                SELECT 	ies.no_ies AS ies,
                        cur.no_curso AS curso,
                        ofe.ofpiescampus AS campus,
                        ofe.ofpano AS ano,
                        REPLACE(ofe.ofpanosemestre, '.', ' - ') AS semestre,
                        hab.habdesc AS habilitação,
                        CASE
                                WHEN ofe.ofpturno = 'M' THEN 'Matutino'
                                WHEN ofe.ofpturno = 'V' THEN 'Vespertino'
                                WHEN ofe.ofpturno = 'N' THEN 'Noturno'
                                WHEN ofe.ofpturno = 'I' THEN 'Integral'
                        ELSE ''
                        END AS turno,
                        ofe.ofpnumvagas AS vagas,
                        to_char(ofe.ofpdtinclusao, 'DD/MM/YYYY') AS inclusão

                FROM assint.ofertacursopecg ofe

                JOIN emec.cursos cur ON cur.co_curso = ofe.co_curso
                JOIN assint.habilitacao hab ON hab.habid = ofe.habid
                JOIN emec.ies ies ON ies.co_ies = ofe.co_ies AND cur.co_ies = ies.co_ies

                WHERE ofpid = {$ofpid};
            ";
            $data = $db->pegaLinha($sql);

            #BUSCA NO BANCO OS DADOS COM OS SEUS RESPECTIVOS VALORES ANTES DA OPERAÇÃO.
            foreach( $data as $key => $campos ){
                if( !is_numeric( $campos ) || $key == 'peccpf' ){
                    $campos = trim($campos);
                }
                $regist_ant[] = $key.':'.$campos;
            }
            $atvdsc_dados_ant = addslashes( implode(';', $regist_ant) );

            $sql = "
                INSERT INTO assint.atividadehistorico(
                        usucpf, ofpid, atvdsc_dados_ant, atvdsc_dados_alt, atvtipo, atvaba, atvdtinclusao
                    )VALUES(
                        '{$usucpf}', {$ofpid}, '{$atvdsc_dados_ant}', '{$atvdsc_dados_alt}', 'D', 'V', 'NOW()'
                ) RETURNING atvid;
            ";
            $atvid = $db->pegaUm($sql);
        }

        #OPERAÇÃO DE ALTERAÇÃO - UPDATE
        if( $tipo == 'A' ){

            $usucpf = $_SESSION['usucpf'];
            $ofpid  = $dados['ofpid'];

            $sql = "
                SELECT 	ies.no_ies AS ies,
                        cur.no_curso AS curso,
                        ofe.ofpiescampus AS campus,
                        ofe.ofpano AS ano,
                        ofe.ofpanosemestre,
                        hab.habdesc AS habilitação,
                        CASE
                                WHEN ofe.ofpturno = 'M' THEN 'Matutino'
                                WHEN ofe.ofpturno = 'V' THEN 'Vespertino'
                                WHEN ofe.ofpturno = 'N' THEN 'Noturno'
                                WHEN ofe.ofpturno = 'I' THEN 'Integral'
                        ELSE ''
                        END AS turno,
                        ofe.ofpnumvagas AS vagas
                        --to_char(ofe.ofpdtinclusao, 'DD/MM/YYYY') AS inclusão

                FROM assint.ofertacursopecg ofe

                JOIN emec.cursos cur ON cur.co_curso = ofe.co_curso
                JOIN assint.habilitacao hab ON hab.habid = ofe.habid
                JOIN emec.ies ies ON ies.co_ies = ofe.co_ies AND cur.co_ies = ies.co_ies

                WHERE ofpid = {$ofpid};
            ";
            $data = $db->pegaLinha($sql);

            #BUSCA NO BANCO OS DADOS COM OS SEUS RESPECTIVOS VALORES ANTES DA ALTERAÇÃO.
            foreach( $data as $key => $campos ){
                if( !is_numeric( $campos ) || $key == 'peccpf' ){
                    $campos = trim($campos);
                }
                $regist_ant[] = $key.':'.$campos;
            }
            $atvdsc_dados_ant = addslashes( implode(';', $regist_ant) );

            #MONTA OS DADOS QUE VEM DO FORMULÁRIO.
            foreach( $dados as $key => $campos ){
                switch($key){
                    case 'co_ies':
                        $key = 'IES';
                        $campos = buscarDadosPec('I', $campos[0]);
                        break;
                    case 'co_curso':
                        $key = 'Curso';
                        $campos = buscarDadosPec('C', $campos);
                        break;
                    case 'ofpiescampus':
                        $key = 'Campus';
                        break;
                    case 'ofpano':
                        $key = 'Ano';
                        break;
                    case 'ofpanosemestre':
                        $key = 'Semestre';
                        break;
                    case 'habid':
                        $key = 'Habilitação';
                        $campos = buscarDadosPec('H', $campos);
                        break;
                    case 'ofpturno':
                        $key = 'Turno';
                        if( $campos == 'M' ){
                            $campos = 'Matutino';
                        }elseif( $campos == 'V' ){
                            $campos = 'Vespertino';
                        }elseif( $campos == 'I' ){
                            $campos = 'Integral';
                        }
                        break;
                    case 'ofpnumvagas':
                        $key = 'Vagas';
                        break;
                }
                if( $key != 'ofpid' && $key != 'requisicao' && $key != 'ofpobservacao' && $key != 'no_ofpobservacao'){
                    $regist_alt[] = $key.':'.str_replace("'", "", $campos);                    
                }
            }
            $atvdsc_dados_alt = addslashes( implode(';', $regist_alt) );

            $sql = "
                INSERT INTO assint.atividadehistorico(
                        usucpf, ofpid, atvdsc_dados_ant, atvdsc_dados_alt, atvtipo, atvaba, atvdtinclusao
                    )VALUES(
                        '{$usucpf}', {$ofpid}, '{$atvdsc_dados_ant}', '{$atvdsc_dados_alt}', 'A', 'V', 'NOW()'
                ) RETURNING atvid;
            ";
            $atvid = $db->pegaUm($sql);
        }

        if( $atvid > 0 ){
            $db->commit();
            return true;
        }else{
            $db->rollback();
            return false;
        }
    }

    function buscarDadosPec( $tipo, $value ){
        global $db;

        if( $tipo == 'H' ){
            $sql = "
                SELECT habdesc FROM assint.habilitacao WHERE habid = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'C' ){
            $sql = "
                SELECT no_curso FROM emec.cursos WHERE co_curso = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'I' ){
            $sql = "
                SELECT no_ies FROM emec.ies WHERE co_ies = {$value};
            ";
            return $db->pegaUm($sql);
        }
        die();
    }

    function recuperarCurso($ofpid){
	global $db;

	$aryWhere[] = "ofpstatus = 'A'";

	if($ofpid){
            $aryWhere[] = "ofpid = {$ofpid}";
	}

	$sql = "
            SELECT  ofpid, 
                    co_curso, 
                    co_ies, 
                    usucpf, 
                    ofpnumvagas, 
                    habid, 
                    ofpdtinclusao, 
                    ofpstatus, 
                    ofpturno, 
                    ofpanosemestre, 
                    ofpobservacao, 
                    ofpano, 
                    ofpiescampus
            FROM assint.ofertacursopecg
            ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
        ";
	$curso = $db->pegaLinha($sql);
	return $curso;
}

function pegaIES(){

    global $db;
    if( !$db->testa_superuser() ){
        $sql = "SELECT
                    co_ies
                FROM
                    assint.usuarioresponsabilidade
                WHERE
                    co_ies IS NOT NULL
                AND
                    rpustatus = 'A'
                AND
                    usucpf = '".$_SESSION['usucpf']."'
                    ";
        return $db->carregarColuna($sql);
    }
}

function pesquisarCurso($co_ies = null, $post = null){
    global $db;

    $perfis = recuperaPerfil();

    $ies = pegaIES();
    
    $ano = $post['ano'];
    
    if(in_array(PERFIL_PEC_G, $perfis) && count($ies) < 1){
        echo '<table class="tabela text-center" cellspacing="1" cellpadding="3" align="center"><tr><td><b>Você não possui instituição vinculada!</b></td></tr></table>';
        return false;
    }


    if($post){
        extract($post);
    }

    $aryWhere[] = "ofe.ofpstatus = 'A'";
    
    if( $ano != '' ){
        $aryWhere[] = " ofe.ofpano = '{$ano}' ";
    }

    if($co_ies[0]){
        $co_ies = implode(',',$co_ies);
        $aryWhere[] = "ofe.co_ies IN ({$co_ies})";
    }

    if($co_curso){
        $aryWhere[] = "ofe.co_curso = {$co_curso}";
    }

    if($habid){
        $aryWhere[] = "ofe.habid = {$habid}";
    }

    if($ofpturno){
        $aryWhere[] = "ofe.ofpturno = '{$ofpturno}'";
    }

    $coluna_ano = array(
        'label' => 'Ano',
        'colunas' => array(
            'Edição do Programa', 'Semestre de Ingresso'
        )
    );

    if(in_array(PERFIL_PEC_G,$perfis) || in_array(PERFIL_SUPER_USUARIO,$perfis) || in_array(PERFIL_ADMINISTRADOR,$perfis)){
        if( $ano == '2015' ){
            $acao = "
                <img src=\"../imagens/alterar.gif\" id=\"' || ofe.ofpid ||'\" class=\"alterar\" onclick=\"alterarCurso('|| ofe.ofpid ||');\" style=\"cursor:pointer;\"/>
                <img src=\"../imagens/excluir_01.gif\" id=\"' || ofe.ofpid ||'\" class=\"excluir\" style=\"cursor:pointer;\"/>
            ";
        }else{
            $acao = "
                <img src=\"../imagens/alterar.gif\" id=\"' || ofe.ofpid ||'\" class=\"alterar\" onclick=\"alterarCurso('|| ofe.ofpid ||');\" style=\"cursor:pointer;\"/>
                <img src=\"../imagens/excluir.gif\" id=\"' || ofe.ofpid ||'\" class=\"excluir\" onclick=\"excluirCurso('|| ofe.ofpid ||');\" style=\"cursor:pointer;\"/>
            ";
        }

        $cabecalho = array('Ação', 'Cód. Inst','Instituição','Cód. Curso','Curso', 'Campus', $coluna_ano, 'Habilitação', 'Turno', 'Nº Vagas', 'Dt. Inclusão');
    } else {
        $acao = "";
        $cabecalho = array('Cód. Inst','Instituição','Cód. Curso','Curso', 'Campus', $coluna_ano, 'Habilitação', 'Turno', 'Nº Vagas');
    }

    $sql = "
        SELECT  '{$acao}'  AS acao,
                ies.co_ies AS co_ies,
                ies.no_ies,
                cur.co_curso,
                cur.no_curso,
                ofe.ofpiescampus,
                ofe.ofpano,
                REPLACE(ofe.ofpanosemestre, '.', '/') AS ofpanosemestre,
                hab.habdesc,
                CASE
                    WHEN ofe.ofpturno = 'M' THEN 'Matutino'
                    WHEN ofe.ofpturno = 'V' THEN 'Vespertino'
                    WHEN ofe.ofpturno = 'N' THEN 'Noturno'
                    WHEN ofe.ofpturno = 'I' THEN 'Integral'
                    ELSE ''
                END AS ofpturno,
                ofe.ofpnumvagas,
                to_char(ofe.ofpdtinclusao, 'DD/MM/YYYY') AS data_inclusao
        FROM assint.ofertacursopecg ofe

        INNER JOIN emec.cursos cur ON cur.co_curso = ofe.co_curso
        INNER JOIN assint.habilitacao hab ON hab.habid = ofe.habid
        INNER JOIN emec.ies ies ON ies.co_ies = ofe.co_ies AND cur.co_ies = ies.co_ies

        ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."

        ORDER BY ies.no_ies, cur.no_curso
    ";
    $alinhamento = Array('left', 'center', 'left', 'center', 'left', 'left', 'center', 'center', 'left', 'left', 'center', 'center' );
    $tamanho = Array('3%', '3%', '20%', '3%', '15%', '15%', '6%', '6%', '5%', '5%', '5%', '5%' );

    $param['ordena'] = true;
    $param['totalLinhas'] = true;
    $param['managerOrder'] = array(
        12  => array('campo' => "ofpdtinclusao", 'alias' => "data_inclusao")
    );


    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento, '', $param);
}

function exibirCurso($post){
	global $db;

	extract($post);

    if($co_ies){
  		$aryWhere[] = "co_ies = {$co_ies}";
   	}

    $sql = "SELECT 			co_curso AS codigo, no_curso AS descricao
    		FROM 			emec.cursos
    						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
    		ORDER BY 		no_curso";

    $db->monta_combo('co_curso',$sql,'S','Selecione um curso','','','','372','S','','');
}

function exibirNomeIES($co_ies){
	global $db;

    if($co_ies){
  		$aryWhere[] = "co_ies = {$co_ies}";
   	}

    $sql = "SELECT 		no_ies
    		FROM 		emec.ies
    					".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')." ";

	$ies = $db->pegaUm($sql);
	return $ies;
}

function gerarExcel($post){
	global $db;

	if($post){
		extract($post);
	}

	$perfis = recuperaPerfil();

	$aryWhere[] = "ofe.ofpstatus = 'A'";

	if($co_ies[0]){
		$co_ies = implode(',',$co_ies);
		$aryWhere[] = "ofe.co_ies IN ({$co_ies})";
	}

	if($co_curso){
		$aryWhere[] = "ofe.co_curso = {$co_curso}";
	}

	if($habid){
		$aryWhere[] = "ofe.habid = {$habid}";
	}

	if($ofpturno){
		$aryWhere[] = "ofe.ofpturno = '{$ofpturno}'";
	}
        if($ano){
                $aryWhere[] = "ofe.ofpano = '{$ano}'";
        }

        $coluna_ano = array(
            'label' => 'Ano',
            'colunas' => array(
                'Edição do Programa', 'Semestre de Ingresso'
            )
        );

	$cabecalho = array('Cód. Inst','Instituição','Cód. Curso','Curso', 'Campus', 'Ano Edição do Programa', 'Ano Semestre de Ingresso', 'Habilitação', 'Turno', 'Nº Vagas', 'Observação');

	$sql = "
            SELECT  ies.co_ies AS co_ies,
                    ies.no_ies,
                    cur.co_curso,
                    cur.no_curso,
                    ofe.ofpiescampus,
                    ofe.ofpano,
                    REPLACE(ofe.ofpanosemestre, '.', ' - ') AS ofpanosemestre,
                    hab.habdesc,
                    CASE
                        WHEN ofe.ofpturno = 'M' THEN 'Matutino'
			WHEN ofe.ofpturno = 'V' THEN 'Vespertino'
			WHEN ofe.ofpturno = 'N' THEN 'Noturno'
			ELSE ''
                    END AS ofpturno,
                    ofe.ofpnumvagas,
                    regexp_replace (ofe.ofpobservacao, '\r|\n', ' ','g') AS ofpobservacao
            FROM assint.ofertacursopecg ofe

            INNER JOIN emec.cursos cur ON cur.co_curso = ofe.co_curso
            INNER JOIN assint.habilitacao hab ON hab.habid = ofe.habid
            INNER JOIN emec.ies ies ON ies.co_ies = ofe.co_ies AND cur.co_ies = ies.co_ies

            ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."

            ORDER BY ies.no_ies, cur.no_curso
        ";
	ob_clean();
	$db->sql_to_excel($sql,"PEC-G_Relatorio",$cabecalho);
}
?>