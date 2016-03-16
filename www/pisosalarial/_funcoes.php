<?php
/**
 * Monta cabeçalho do ente federado e orgão
 * @name montaCabecalho
 * @author Wesley Romualdo da Silva
 * @return string
 */
function montaCabecalho()
{
	global $db;

	if(possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL, PERFIL_CONSULTA_MUNICIPAL)) && !possuiPerfil(array(PERFIL_ADMINISTRADOR)) && !$db->testa_superuser()){

        $sql = "SELECT DISTINCT
                  CASE WHEN ent.entnome IS NOT NULL THEN ent.entnome
                  ELSE usu.orgao END as orgao,
                  mu.mundescricao,
                  mu.estuf
                FROM
                  seguranca.usuario usu
                  left join pisosalarial.usuarioresponsabilidade ur on usu.usucpf = ur.usucpf
                  --left join public.orgao org on org.orgcod = usu.orgcod
                  left join territorios.municipio mu on mu.muncod = ur.muncod
                  left join entidade.entidade ent on ent.entid = usu.entid
                WHERE usu.usucpf = '".$_SESSION['usucpf']."'";
	}else{

	    $sql = "SELECT DISTINCT
                  '' as orgdsc,
                  mu.mundescricao,
                  mu.estuf
                FROM territorios.municipio mu
                where mu.muncod = '".$_SESSION['piso']['muncod']."'";

	}

	$arDados = $db->pegaLinha( $sql );

	if(count($arDados)){
    	echo '<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    				<tr>
    					<td class="SubtituloDireita" style="width: 19%">Ente Federado:</td>
    					<td>'.$arDados['mundescricao'].'/'.$arDados['estuf'].'</td>
    				</tr>';
    	if(!possuiPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_CONSULTA_GERAL))){
        	echo	'<tr>
        				<td class="SubtituloDireita">Orgão:</td>
        				<td>'.$arDados['orgao'].'</td>
        			</tr>';
    	}
    	if(!empty($_REQUEST['anoref']) && !empty($_REQUEST['mesref'])){
            echo    '<tr>
                        <td class="SubtituloDireita">Mês / Ano de Referência:</td>
                        <td>'.$_REQUEST['mesref'].'/'.$_REQUEST['anoref'].'</td>
                    </tr>';
    	}
    	echo '</table>';
	}
}

function gravaDadosSessao()
{
    global $db;

    if(possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL, PERFIL_CONSULTA_MUNICIPAL), false) && !possuiPerfil(array(PERFIL_ADMINISTRADOR))){

        $sql = "SELECT DISTINCT
                  --org.orgdsc,
                  mu.mundescricao,
                  mu.muncod,
                  mu.estuf
                FROM
                  seguranca.usuario usu
                  left join pisosalarial.usuarioresponsabilidade ur on usu.usucpf = ur.usucpf
                  --left join public.orgao org on org.orgcod = usu.orgcod
                  left join territorios.municipio mu on mu.muncod = ur.muncod
                where usu.usucpf = '".$_SESSION['usucpf']."'";

        $arDados = $db->pegaLinha( $sql );

        if(empty($_SESSION['piso']['muncod']) || ($_SESSION['piso']['muncod'] != $arDados['muncod'])){
            $_SESSION['piso']['estuf']  = $arDados['estuf'];
            $_SESSION['piso']['muncod'] = $arDados['muncod'];
        }
    }
}

function recuperarArrayPerfis($usucpf)
{
    global $db;

    $sql = "SELECT
                pu.pflcod
            FROM
                seguranca.perfil AS p
            LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
            WHERE
                p.sisid = '{$_SESSION['sisid']}'
                AND pu.usucpf = '$usucpf'";

    $pflcod = $db->carregar( $sql );

    foreach($pflcod as $dados){
        $arPflcod[] = $dados['pflcod'];
    }

    return $arPflcod;
}

function possuiPerfil( $pflcods, $testa_su = true){

    global $db;

    if($db->testa_superuser() && $testa_su){
        return true;
    }

    if ( is_array( $pflcods ) ){
        $pflcods = array_map( "intval", $pflcods );
        $pflcods = array_unique( $pflcods );
    } else {
        $pflcods = array( (integer) $pflcods );
    } if ( count( $pflcods ) == 0 ) {
        return false;
    }
    $sql = "SELECT
                    count(*)
            FROM seguranca.perfilusuario
            WHERE
                usucpf = '" . $_SESSION['usucpf'] . "' and
                pflcod in ( " . implode( ",", $pflcods ) . " ) ";
    return $db->pegaUm( $sql ) > 0;
}

function formata_valor_sql($valor){

    $valor = str_replace('.', '', $valor);
    $valor = str_replace(',', '.', $valor);

    return $valor;

}

/*********************************************************/
/*************** FUNÇÕES DO WORKFLOW *********************/
/*********************************************************/

function criarDocumento( $muncod ) {
	global $db;

	$docid = pegarDocid($muncod);

	if( !$docid ) {

		$tpdid = TPDID_PISOSALARIAL;

		// descrição do documento
		$docdsc = "Cadastro de Folha de Pagamento (pisosalarial) - n°" . $muncod;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		$sql = "SELECT pmuid FROM pisosalarial.pisomunicipio WHERE muncod = '".$muncod."'";

		if( $db->pegaUm( $sql ) ){
			// atualiza o plano de trabalho
			$sql = "UPDATE
						pisosalarial.pisomunicipio
					SET
						docid = ".$docid."
					WHERE
						muncod = '".$muncod."'";

			$db->executar( $sql );
		} else {
			$sql = "INSERT INTO pisosalarial.pisomunicipio(docid, muncod)
					VALUES ('{$docid}', '{$muncod}')";
			$db->executar( $sql );
		}
		$db->commit();
	}

	return $docid;
}

function pegarDocid( $muncod ) {
	global $db;

	$sql = "SELECT
				docid
			FROM
				pisosalarial.pisomunicipio
			WHERE
			 	muncod = '".$muncod."'";

	return (integer) $db->pegaUm( $sql );
}

function pegarEstadoAtual( $muncod ) {
	global $db;

	$docid = pegarDocid( $muncod );

	$sql = "select
				ed.esdid
			from
				workflow.documento d
			inner join
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;
}

/**
 * Verifica se tem ano referencia se não tiver exibe javascript redirecionando.
 */
function verificaAnoRef(){
    if(empty($_REQUEST['anoref'])){

        echo "
            <script type='text/javascript'> 
                alert('Ano de referencia do município não encontrado.');
                document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
            </script>";
        exit;
    }
}

function verificaMuncod()
{
    if(empty($_SESSION['piso']['muncod'])){

        echo "<script>
                alert('Código do município não encontrado.');
                document.location.href = 'pisosalarial.php?modulo=inicio&acao=C';
              </script>";
        exit;
    }
}

function truncarValor($valor = 0, $decimais = 2)
{

    $arValor = explode(".", $valor);
    $valor   = $arValor[0].".".substr($arValor[1],  0, $decimais);

    return $valor;
}

function eval_syntax($code)
{
    $braces = 0;
    $inString = 0;

    // We need to know if braces are correctly balanced.
    // This is not trivial due to variable interpolation
    // which occurs in heredoc, backticked and double quoted strings
    foreach (token_get_all('<?php ' . $code) as $token)
    {
        if (is_array($token))
        {
            switch ($token[0])
            {
            case T_CURLY_OPEN:
            case T_DOLLAR_OPEN_CURLY_BRACES:
            case T_START_HEREDOC: ++$inString; break;
            case T_END_HEREDOC:   --$inString; break;
            }
        }
        else if ($inString & 1)
        {
            switch ($token)
            {
            case '`':
            case '"': --$inString; break;
            }
        }
        else
        {
            switch ($token)
            {
            case '`':
            case '"': ++$inString; break;

            case '{': ++$braces; break;
            case '}':
                if ($inString) --$inString;
                else
                {
                    --$braces;
                    if ($braces < 0) return false;
                }

                break;
            }
        }
    }

    if ($braces) return false; // Unbalanced braces would break the eval below
    else
    {
        ob_start(); // Catch potential parse error messages
        $code = eval('if(0){' . $code . '}'); // Put $code in a dead code sandbox to prevent its execution
        ob_end_clean();

        return false !== $code;
    }
}

function verificaCondicaoParametroPiso($parametro, $sigla, $indice, $coluna, $ponto, &$valor = 0, &$arPontos = array())
{
    $parametro = trim($parametro);
    if(!empty($parametro)){

        $formula = str_replace('ou','||',str_replace($sigla, $indice, $parametro));
        if(eval_syntax("if(".trim($formula)."){}")){

            if(eval("return (".$formula.");")){
                echo '<span title="'.$parametro.'" alt="'.$parametro.'">'.$ponto.'</span>';
                $valor = $valor+$ponto;
                $arPontos[$coluna] = 'true';
            }else{
                echo '&nbsp;';
            }

        }else{

            echo "fórmula incorreta";
        }

    }else{
        echo '&nbsp;';
    }
}

function validaEnvioAnalise($url, $muncod){
	global $db;
	
	$sql = "SELECT count(gerid) FROM pisosalarial.gestaorecurso WHERE muncod = '$muncod'";
	$gerid = $db->pegaUm( $sql );
	$sql = "SELECT count(plcid) FROM pisosalarial.planocarreira WHERE muncod = '$muncod'";
	$plcid = $db->pegaUm( $sql );
	
	$sql = "SELECT count(fp.flpid) FROM 
			  pisosalarial.folhapagamento fp
			  inner join pisosalarial.pisomunicipio pm on pm.pmuid = fp.pmuid
			WHERE
				pm.muncod = '$muncod'
    			and fp.flpfinalizado = false";
	
	$boFinalizado = $db->pegaUm( $sql );
	if( $boFinalizado == 0 && $gerid != 0 && $plcid != 0 ){
		return true;
	} else {
		if( $gerid == 0 ) return "Documentação referente à Gestão de recursos não foi encontrada. Operação cancelada!";
		if( $plcid == 0 ) return "Documentação referente ao Plano de Carreira não foi  encontrada. Operação cancelada!";
		if( $boFinalizado != 0 ) return "Enquanto existir uma ou mais folha de pagamento em aberto o Piso salário não será enviado para análise. Operação cancelada!";
		//return false;
	}
	
}
?>