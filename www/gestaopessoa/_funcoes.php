<?php

/*** Exibe a lista com os membros a serem avaliados ***/

//nova - refeita
function listaEquipeAvaliacao($usucpf) {
    global $db;

    /*     * * Recupera os dados do usuário ** */
    $dadosUsuario = $db->carregar("SELECT * FROM gestaopessoa.servidor WHERE sercpf = '" . $usucpf . "' AND seranoreferencia = " . $_SESSION['exercicio'] . " AND serstatus=TRUE");

    /*     * * Se existem dados na tabela 'gestaopessoa.servidor' ** */

    if (!is_null($dadosUsuario)) {
        /*         * * Recupera o CPF's dos membros da equipe a ser avaliada ** */
        $equipeAvaliacao = equipeAvaliacao($dadosUsuario[0]['sercpf']);
        //ver($equipeAvaliacao);
        /*         * * Se retornou algum membro da equipe... ** */
        if (!is_null($equipeAvaliacao)) {
            /*             * * Monta o array com as condições ** */
            $where = array();
            $where[] = "s.serstatus=TRUE";
            $where[] = "s.sercpf IN ('" . implode("','", $equipeAvaliacao) . "')";
            $where[] = "s.seranoreferencia = " . $_SESSION['exercicio'] . "";
            $where[] = "s.tssid NOT IN (" . REQUISITADO . ", " . NOMEADO_CARGO_COMIS . ", " . REQ_DE_OUTROS_ORGAOS . ", " . EXERC_DESCENT_CARREI . ")";

            if ($_POST['filtro_nome']) {
                $where[] = "s.sernome ILIKE '%" . $_POST['filtro_nome'] . "%'";
            }
            if ($_POST['filtro_cpf']) {
                $where[] = "s.sercpf ILIKE '%" . $_POST['filtro_cpf'] . "%'";
            }
            if ($_POST['filtro_siape']) {
                $where[] = "s.sersiape ILIKE '%" . $_POST['filtro_siape'] . "%'";
            }

            /*             * * Recupera cpf do chefe ** */
            $cpfChefe = $db->pegaUm("SELECT sercpfchefe FROM gestaopessoa.servidor WHERE sercpf = '" . $usucpf . "' AND seranoreferencia = " . $_SESSION['exercicio'] . " AND serstatus=TRUE");

            $alterarPessoaAutoAval = "<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarPessoaAutoAval(\'' || s.sercpf || '\');\" title=\"Selecionar Pessoa\">";

            $alterarPessoa = "<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarPessoa(\'' || s.sercpf || '\');\" title=\"Selecionar Pessoa\">";

            /*             * * Monta o SQL ** */
            $sql = "
                SELECT  DISTINCT

                        CASE WHEN s.sercpf = '" . $dadosUsuario[0]['sercpf'] . "'
                            THEN '$alterarPessoaAutoAval'
							ELSE '$alterarPessoa'
						END AS acao,

                        s.sercpf AS cpf,
                        s.sernome AS nome,

                        CASE
                            WHEN s.sercpf = '" . $dadosUsuario[0]['sercpf'] . "' THEN 'Auto'
                            WHEN  (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) != 0
                                THEN 'Chefe'
                                ELSE 'chefiado'
                        END as hierarquia,
                        s.sersiape AS siape
					FROM
			       		gestaopessoa.servidor s
				    WHERE
				    	" . implode(' AND ', $where) . " order by s.sernome";

            /*             * * Monta o a lista ** */
            //ver($sql);
            $cabecalho = array("Ação", "CPF", "Nome", "Hierarquia", "SIAPE");
            $db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', '');
        }
    }
}

/*** Retorna um array com o CPF de todos os membros da equipe ***/
function membrosEquipe($usucpf) {
    return equipeAvaliacao($usucpf, true);
    /*     * * Usa o objeto do banco ** */
    //global $db;
    /*     * * Inicializa a variável de retorno ** */
    //$membrosEquipe = null;
    /*     * * Recupera os dados do usuário ** */
    //$dadosUsuario = $db->carregar("SELECT * FROM gestaopessoa.servidor WHERE sercpf = '".$usucpf."' AND seranoreferencia = ".$_SESSION['exercicio']);
    /*     * * Se existem dados na tabela 'gestaopessoa.servidor' ** */
    //if( !is_null($dadosUsuario) )
    //{
    /*     * * Se for DAS 4 ** */
    //if( (integer)$dadosUsuario[0]['sernivelfuncao'] == 1014 )
    //{
    // Não tem equipe
    //}
    /*     * * Se for DAS 3 ** */
    //elseif( (integer)$dadosUsuario[0]['sernivelfuncao'] == 1013 )
    //{
    /*     * * Todos os DAS 2 ** */
    //$sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '".$dadosUsuario[0]['sercpf']."' AND seranoreferencia = ".$_SESSION['exercicio'];
    //$membrosEquipe = $db->carregarColuna($sql);
    //if( is_null($membrosEquipe) ) $membrosEquipe = array();
    //}
    /*     * * Se for DAS 2 ** */
    //elseif( (integer)$dadosUsuario[0]['sernivelfuncao'] == 1012 )
    //{
    /*     * * Todos os seus subordinados ** */
    //$sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '".$dadosUsuario[0]['sercpf']."' AND seranoreferencia = ".$_SESSION['exercicio'];
    //$membrosEquipe = $db->carregarColuna($sql);
    //if( is_null($membrosEquipe) ) $membrosEquipe = array();
    /*     * * O seu chefe ** */
    //if( !is_null($dadosUsuario[0]['sercpfchefe']) && $dadosUsuario[0]['sercpfchefe'] != '' )
    //{
    //array_push($membrosEquipe, $dadosUsuario[0]['sercpfchefe']);
    //}
    //}
    /*     * * Se tiver qualquer outra função, ou não tiver função... ** */
    //else
    //{
    // Todos os seus subordinados, se houver ***/
    /*
      $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '".$dadosUsuario[0]['sercpf']."' AND seranoreferencia = ".$_SESSION['exercicio'];
      $membrosEquipe = $db->carregarColuna($sql);
      if( is_null($membrosEquipe) ) $membrosEquipe = array();
      // Todos os membros de sua equipe (mesmo nível), se houver /
      if( !is_null($dadosUsuario[0]['sercpfchefe']) && $dadosUsuario[0]['sercpfchefe'] != '' )
      {
      $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '".$dadosUsuario[0]['sercpfchefe']."' AND sercpf <> '".$dadosUsuario[0]['sercpf']."' AND seranoreferencia = ".$_SESSION['exercicio'];
      $equipeMembros = $db->carregarColuna($sql);
      if( !is_null($equipeMembros) )
      {
      $membrosEquipe = array_merge($membrosEquipe, $equipeMembros);
      }
      }
      // O seu chefe /
      if( !is_null($dadosUsuario[0]['sercpfchefe']) && $dadosUsuario[0]['sercpfchefe'] != '' )
      {
      array_push($membrosEquipe, $dadosUsuario[0]['sercpfchefe']);
      }
      //}
      }

      if( is_array($membrosEquipe) )
      {
      if( empty($membrosEquipe) )
      {
      $membrosEquipe = null;
      }
      }

      return $membrosEquipe;
     */
}

function testaSuperUsuario($usucpf) {
    $perfis = arrayPerfil($usucpf);

    if (is_array($perfis)) {
        foreach ($perfis as $perfil) {
            if ($perfil == PERFIL_SUPER_USER) {
                return true;
            }
        }
        return false;
    }
}

#RETORNA UM ARRAY COM O CPF DE TODAS AS PESSOAS A SEREM AVALIADAS.
function equipeAvaliacao($usucpf, $menosAPropriaPessoa = null, $verificaSeServidorExclusivo = null) {
    global $db;

    #SE SUPER USUÁRIO.
    if (testaSuperUsuario($usucpf)) {
        #PEGA TODOS OS USUÁRIO E MOSTRA NA LISTA.
        $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']}";
        $equipe = $db->carregarColuna($sql);
        return $equipe;
    }

    #INICIALIZA A VARIÁVEL DE RETORNO.
    $equipeAvaliacao = null;
    #RECUPERA OS DADOS DO USUÁRIO.
    $dadosUsuario = $db->carregar("SELECT * FROM gestaopessoa.servidor WHERE sercpf = '{$usucpf}' AND seranoreferencia = {$_SESSION['exercicio']} AND serstatus = true");

    #SE EXISTEM DADOS NA TABELA 'GESTAOPESSOA.SERVIDOR'.
    if (!is_null($dadosUsuario)) {
        #AVALIA A TODOS OS SEUS SUBORDINADOS, SE HOUVER.
        $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '{$dadosUsuario[0]['sercpf']}' AND seranoreferencia = {$_SESSION['exercicio']} AND tssid NOT IN (3, 4, 14, 18) AND serstatus = true";
        $equipeAvaliacao = $db->carregarColuna($sql);

        if (is_null($equipeAvaliacao)) {
            $equipeAvaliacao = array();
        }

        #AVALIA OS MEMBROS DE SUA EQUIPE (MESMO NÍVEL)
        if (!is_null($dadosUsuario[0]['sercpfchefe']) && $dadosUsuario[0]['sercpfchefe'] != '') {
            $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '{$dadosUsuario[0]['sercpfchefe']}' AND sercpf <> '{$dadosUsuario[0]['sercpf']}' AND seranoreferencia = " . $_SESSION['exercicio'] . " AND tssid NOT IN (3, 4, 14, 18) AND serstatus = true";
            $equipeMembros = $db->carregarColuna($sql);

            $sql = "SELECT sernivelfuncao FROM gestaopessoa.servidor WHERE sercpfchefe = '{$dadosUsuario[0]['sercpfchefe']}' AND sercpf <> '{$dadosUsuario[0]['sercpf']}' AND seranoreferencia = {$_SESSION['exercicio']} AND serstatus = true";
            $cargoDAS = $db->carregarColuna($sql);

            #VERIFICA SE OS MEMBROS TEM ALGUM SUBORDINADO.
            foreach ($equipeMembros as $chave => $membros) {
                if ($cargoDAS[$chave] != 1013 || $cargoDAS[$chave] != 1014 || $cargoDAS[$chave] != 1015 || $cargoDAS[$chave] != 1016) {
                    $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpfchefe = '{$membros}' AND seranoreferencia = {$_SESSION['exercicio']} AND tssid NOT IN (3, 4, 14, 18) AND serstatus = true";
                    $subordinados = $db->carregarColuna($sql);
                    if (is_array($subordinados)) {
                        #SE EXISTE SUBORDINADO RETIRO A PESSOA DA LISTA DE AVALIAÇÃO.
                        if ($subordinados[0]) {
                            unset($equipeMembros[$chave]);
                        }
                    }
                }
            }

            if (is_array($equipeAvaliacao)) {
                #SE FOR CHEFE NÃO AVALIA COMPANHERIOS DE MESMA CHEFIA (AO LADO)
                if ($equipeAvaliacao[0]) {
                    unset($equipeMembros);
                    $equipeMembros = array();
                }
            }

            if (!is_null($equipeMembros)) {
                $equipeAvaliacao = array_merge($equipeAvaliacao, $equipeMembros);
            }
        }

        #SE AUTO-AVALIA
        $sql = "SELECT sernivelfuncao FROM gestaopessoa.servidor WHERE sercpf = '{$dadosUsuario[0]['sercpf']}' AND seranoreferencia = {$_SESSION['exercicio']} AND serstatus = true";
        $cargoDAS = $db->pegaUm($sql);
        $cargoDAS = $cargoDAS == 0 ? '' : $cargoDAS;

        if ($menosAPropriaPessoa !== true) {
            if (($cargoDAS != 1014) && ($cargoDAS != 1015) && ($cargoDAS != 1016)) {
                array_push($equipeAvaliacao, $dadosUsuario[0]['sercpf']);
            }
        }

        #AVALIA O SEU CHEFE.
        if (!is_null($dadosUsuario[0]['sercpfchefe']) && $dadosUsuario[0]['sercpfchefe'] != '') {
            if( ($cargoDAS != 1014) && ($cargoDAS != 1015) && ($cargoDAS != 1016) /* && ($cargoDAS != 1013) && ($cargoDAS != 1022) && $cargoDAS */) {
                array_push($equipeAvaliacao, $dadosUsuario[0]['sercpfchefe']);
            }
        }
    }

    if (is_array($equipeAvaliacao)) {
        if (empty($equipeAvaliacao)) {
            $equipeAvaliacao = null;
        }
    }

    #MOSTRA COLUNA DE AVALIAÇÃO DE EQUIPE - SE O USUÁRIO SO TEM ELE E O CHEFE.
    if ($equipeAvaliacao != null) {
        $sql = "
            SELECT DISTINCT s.sercpf AS cpf
            FROM gestaopessoa.servidor s

            WHERE s.sercpf IN ('" . implode("','", $equipeAvaliacao) . "')
                AND s.seranoreferencia = {$_SESSION['exercicio']}
                AND s.tssid NOT IN (". REQUISITADO .", ". NOMEADO_CARGO_COMIS ." , ". REQ_DE_OUTROS_ORGAOS .", ". EXERC_DESCENT_CARREI .")
                AND  s.serstatus = true
        ";
        $semEquipe = $db->carregarColuna($sql);
    } else {
        $semEquipe = array();
    }

    #SE O USUÁRIO NÃO FAZ PARTE DE NENHUMA EQUIPE - USUÁRIO SOLTO NA ADMINSTRAÇÃO
    $sql = "
        SELECT DISTINCT sercomequipe
        FROM gestaopessoa.servidor s
        WHERE s.sercpf IN ('". $dadosUsuario[0]['sercpf'] ."') AND s.serstatus = true
            AND s.seranoreferencia = {$_SESSION['exercicio']}
            AND s.tssid NOT IN (". REQUISITADO .", ". NOMEADO_CARGO_COMIS .", ". REQ_DE_OUTROS_ORGAOS .", ". EXERC_DESCENT_CARREI .")
    ";
    $sersemequipe = $db->pegaUm($sql);

    if ($sersemequipe == 'f' && $verificaSeServidorExclusivo == true) {
        return TRUE;
    }

    $sql = "
        SELECT DISTINCT s.sercpf AS cpf
        FROM gestaopessoa.servidor s

        WHERE s.sercpf IN ('". implode("','", $equipeAvaliacao) ."')

        AND s.seranoreferencia = " . $_SESSION['exercicio'] . "
		AND s.tssid NOT IN (". REQUISITADO .", ". NOMEADO_CARGO_COMIS .", ". REQ_DE_OUTROS_ORGAOS .", ". EXERC_DESCENT_CARREI .")
    ";
    $equipeFim = $db->carregarColuna($sql);

    if ($equipeFim && $verificaSeServidorExclusivo == true) {
        if (count($equipeFim) == 1) {
            foreach ($equipeAvaliacao as $chave => $cpf) {
                if ($dadosUsuario[0]['sercpfchefe'] == $cpf) {
                    return true;
                }
            }
            return false;
        } else {
            foreach ($equipeAvaliacao as $chave => $cpf) {
                if ($dadosUsuario[0]['sercpfchefe'] == $cpf) {
                    if ($usucpf == $cpf) {
                        return true;
                    }
                }
            }
            return false;
        }
    }
    #FIM DE MOSTRA COLUNA DE AVALIAÇÃO DE EQUIPE
    return $equipeAvaliacao;
}

function cabecalhoPessoa($usucpf){
    global $db;
    $sql = "
        SELECT su.usunome FROM gestaopessoa.ftdadopessoal
        LEFT JOIN seguranca.usuario AS su ON su.usucpf = '{$usucpf}'
        LIMIT 1
    ";
    $dados = $db->carregar( $sql );

    if( $dados ){
        $cabecalho = "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
        $cabecalho .= "
            <tr>
                <td class =\"SubTituloDireita\" align=\"right\">Nome</td>
                <td>".$dados[0]['usunome']."</td>
            </tr>
            <tr>
                <td class =\"SubTituloDireita\" align=\"right\">CPF</td>
                <td>". formatar_cpf($usucpf)."</td>
            </tr>";
        $cabecalho .= "</table>";
    }
    return $cabecalho;
}

function getAvaliadorHTML( $tipo , $peso = false, $pesoa = false, $pesoc = false, $indice = false, $defid = false){
    global $db;

    #BUSCA O TIPO DE GRATIFICAÇÃO QUE É ATRIBUIDA AO SERVIDOR AVALIADO.
    $resp = verificaTipoGratificacaoServidor( $_SESSION['cpfavaliado'] );

    #CARREGANDO TIPO (1-AUTOAVALIAÇÃO/ 2-AVALIAÇÃO SUPERIOR/ 3-CONSENSO)
    $sql = "SELECT tavid, tavdescricao FROM gestaopessoa.tipoavaliador WHERE tavstatus = 'A'";
    $rs  = $db->carregar( $sql );

    $perfis = arrayPerfil();

    #VERIFICA SE O USUARIO FAZ PARTE DE UMA EQUIPE OU SE É UM USUARIO "SOLTO"

    $usuarioExclusivo = equipeAvaliacao($_SESSION['cpfavaliado'], null, true);

    if( $rs ){
        for( $k = 0; $k < count( $rs ); $k++ ){
            if( $tipo == 'TIPO_CABECALHO'){
                #VERIFICO SE É AVALIAÇÃO DE EQUIPE, CASO SEJA, VERIFICA SE EXISTE AVALIAÇÃO FEITA POR ALGUM INTEGRANTE DA EQUIPE, CASO SIM ESCREVE O CABEÇALHO.
                if( $rs[$k]['tavid'] == 3){
                    if( existeAvaliacaoTipo( $_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO ) ){
                        echo "<th>". $rs[$k]['tavdescricao'] ."</th>";
                        echo "<th> Nota Final </th>";
                    }
                }else{

                    if( $rs[$k]['tavid'] == 1  ){
                        if( ($resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS') ){
                            echo "<th>". $rs[$k]['tavdescricao'] ."</th>";
                            echo "<th> Nota Final </th>";
                        }
                    }else{
                        echo "<th>". $rs[$k]['tavdescricao'] ."</th>";
                        echo "<th> Nota Final </th>";
                    }
                }

            }elseif( $tipo == 'TIPO_COLUNA'){

                $mascaraGlobalJs = "this.value=mascaraglobal('###',this.value);";

                $disabled = "disabled = disabled";

                $valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']);

                $calculoTotal = verificaQuantidade();

                if($rs[$k]['tavid'] == 2){
                    $pesoCalculado = round( $valor * $peso );
                }elseif($rs[$k]['tavid'] == 1){
                    $pesoCalculado = round( $valor * $pesoa );
                }else{
                    $pesoCalculado = round( $valor * $pesoc );
                    /*
                    if( !$calculoTotal ){
                        $pesoCalculado = 0;
                    }
                    */
                }

                $pesoCalculado = $pesoCalculado ? str_pad($pesoCalculado, 3, 0, STR_PAD_LEFT) : '';
                $valor = $valor ? str_pad($valor, 3, 0, STR_PAD_LEFT) : '';

                #VERIFICO SE A "RODADA" É DO TIPO 3 (AVALIAÇÃO DE EQUIPE), CASO SEJA, VEIFICA SE EXISTE AVALIAÇÃO, SE SIM "EXIBE" ESCREVE O CAMPO.
                if( $rs[$k]['tavid'] != 3){
                    if( $rs[$k]['tavid'] == 1  ){
                         if( ($resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS') ){
            ?>
                            <td align="center">
                                <input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>">
                            </td>
                            <td align="center">
                                <div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div>
                            </td>
            <?PHP
                         }
                    }else{
            ?>
                            <td align="center">
                                <input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>">
                            </td>
                            <td align="center">
                                <div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div>
                            </td>
            <?PHP
                    }
                }else{

                    if( existeAvaliacaoTipo( $_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO ) ){
            ?>
                        <td align="center">
                            <input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>">
                        </td>
                        <td align="center">
                            <div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div>
                        </td>
            <?PHP
                    }
                }
            }//fim elseif
        }//fim fo
    }//fim if ($rs)
}//fim function

function getAvaliadorRodapeHTML() {
    global $db;

    #BUSCA O TIPO DE GRATIFICAÇÃO QUE É ATRIBUIDA AO SERVIDOR AVALIADO.
    $resp = verificaTipoGratificacaoServidor( $_SESSION['cpfavaliado'] );

    #VERIFICA SE O USUARIO FAZ PARTE DE UMA EQUIPE OU SE É UM USUARIO "SOLTO". CASO O USUARIO FAÇA PARTE DE UMA EQUIPE RETORNA TRUE.
    $usuarioExclusivo = equipeAvaliacao($_SESSION['cpfavaliado'], null, true);

    $sql = "SELECT tavid, tavdescricao FROM gestaopessoa.tipoavaliador WHERE tavstatus = 'A'";
    $rs = $db->carregar($sql);
    $perfis = arrayPerfil();

    if ($rs) {
        for ($k = 0; $k < count($rs); $k++) {

            if ( ( in_array(PERFIL_AVAL_SERV_AVALIACAO, $perfis) && !soConsulta()) ){
                if (!existeNotaAvaliado($_SESSION['cpfavaliado'])) {

                    if ($rs[$k]['tavid'] == TIPO_AUTO_AVAL) {
                        if( ($resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS') ){
?>
                        <td align="center" id="total_auto_aval" > </td>
                        <td align="center"><div id="total_auto_aval_p" style="display: '';"></div></td>
<?PHP
                        }
                    } elseif ($rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR) {
?>

                        <td align="center" id="total_aval_superior"> </td>
                        <td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>
<?PHP
                    } else {
                        if (existeAvaliacaoTipo($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO)) {
?>
                        <td align="center" id="total_consenso" > </td>
                        <td align="center"><div id="total_consenso_p" style="display: '';"></div></td>
<?PHP
                        }
                    }
                } elseif ($rs[$k]['tavid'] == TIPO_AUTO_AVAL) {
                    if( ($resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS') ){
?>
                        <td align="center" id="total_auto_aval"> </td>
                        <td align="center"><div id="total_auto_aval_p" style="display: '';"></div></td>
<?PHP
                    }
                } elseif ($rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR) {
?>
                        <td align="center" id="total_aval_superior"> </td>
                        <td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>
<?PHP
                } else {
                    if (existeAvaliacaoTipo($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO)) {
?>
                        <td align="center" id="total_consenso"> </td>
                        <td align="center"><div id="total_consenso_p" style="display: '';"></div></td>
<?PHP
                    }
                }
            } else {
                if( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ){
                    if( ($resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS') ){
?>
                        <td align="center" id="total_auto_aval"> </td>
                        <td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>
<?PHP
                    }
                } elseif ($rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR) {
?>
                        <td align="center" id="total_aval_superior"> </td>
                        <td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>
<?PHP
                } else {
                    if (existeAvaliacaoTipo($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO)) {
?>
                        <td align="center" id="total_consenso"> </td>
                        <td align="center"><div id="total_consenso_p" style="display: '';"></div></td>
<?PHP
                    }
                }
            }
        }
    }
}

function verificaQuantidade() {
    global $db;

    $membrosEquipe = equipeAvaliacao($_SESSION['cpfavaliado'], true);

    $sql = "
        SELECT DISTINCT ra.resavaliacpf
        FROM gestaopessoa.respostaavaliacao ra
        JOIN gestaopessoa.servidor sr ON sr.sercpf = ra.resavaliacpf AND sr.serstatus = true AND ra.resano = sr.seranoreferencia

        WHERE ra.resano = {$_SESSION['exercicio']} AND ra.sercpf = '{$_SESSION['cpfavaliado']}' AND ra.resavaliacpf <> '{$_SESSION['cpfavaliado']}'
    ";
    $membrosResponderam = $db->carregarColuna($sql);

    if (count($membrosEquipe) === count($membrosResponderam))
        return true;
    else
        return false;
}

function existeNotaAvaliado($cpf) {
    global $db;
    $sql = "
        SELECT resnota
        FROM gestaopessoa.respostaavaliacao
        WHERE sercpf = '{$cpf}' AND tavid = ".TIPO_AVAL_SUPERIOR." AND resavaliacaopendente = 'f' AND resano = {$_SESSION['exercicio']}
    ";
    $existe = $db->pegaUm($sql);
    if ($existe) {
        return true;
    } else {
        return false;
    }
}

function avaliacaoFinalizada($cpf, $tipo){
    global $db;

    $funcionarioExclusivo = equipeAvaliacao($_SESSION['cpfavaliado'], null, true);

    if ($funcionarioExclusivo == true && $tipo == 3) {
        return true;
    }


    #ENQUANTO NÃO HOUVER DEFINIÇÃO DE REGRAS PARA O BOTÃO FINALIZAR O ARGUMENTO "--AND r.resavaliacaopendente = 'f'" DEVE FICAR COMENTADO.
    $sql = "
        SELECT  resavaliacaopendente
        FROM gestaopessoa.respostaavaliacao AS r
        INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
        WHERE r.sercpf = '{$cpf}' AND d.defanoreferencia = {$_SESSION['exercicio']} AND r.tavid = {$tipo} --AND r.resavaliacaopendente = 'f'
    ";
    $existe = $db->pegaUm($sql);

    if ($existe) {
        return true;
    } else {
        return false;
    }
}

//refeita - nova
function getDadosPessoa($cpf) {
    global $db;

    $sql = "
        SELECT  s.sernome,
        s.sersiape,
        s.sercargo ,
        s.sercpfchefe,
        s2.sernome as chefe,
        u.usucpf
        FROM gestaopessoa.servidor AS s
        LEFT JOIN seguranca.usuario AS u ON u.usucpf = s.sercpf
        LEFT JOIN gestaopessoa.servidor as s2 on s.sercpfchefe = s2.sercpf
        WHERE u.usucpf = '$cpf' AND s2.seranoreferencia = {$_SESSION['exercicio']} AND s.seranoreferencia = {$_SESSION['exercicio']}";

    $rs = $db->carregar($sql);
    if ($rs) {
        $dados = array();
        array_push($dados, $rs[0]['usucpf'], $rs[0]['sernome'], $rs[0]['sersiape'], $rs[0]['sercargo'], $rs[0]['chefe']);
    }
    return $dados;
}

function existeServidorUsuario() {
    global $db;
    $sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpf = '" . $_SESSION['usucpf'] . "'";
    $existe = $db->pegaUm($sql);
    if ($existe) {
        return true;
    } else {
        return false;
    }
}

function direcionaAvaliador($usucpf) {
    global $db;
    $sql = "SELECT s.sercpf FROM gestaopessoa.servidor AS s
                        INNER JOIN seguranca.usuario AS u ON u.usucpf = s.sercpf
                        WHERE s.sercpfchefe = '$usucpf'";
    $existe = $db->pegaUm($sql);

    $perfis = arrayPerfil($usucpf);
    if ($existe || in_array(PERFIL_SUPER_USER, $perfis)) {
        return true;
    } elseif (!existeServidorUsuario()) {
        //echo "<script>alert('Servidor não cadastrado')</script>";
        echo'<script> alert(\'Servidor não cadastrado, Favor encaminhar para o link de RH de sua Unidade os seguintes dados:\n\nCPF\nSIAPE\nNome Servidor\nCargo\nSituaçao Funcional\nfunçao\nLotação\nCPF Chefia\nNome Chefia\nSIAPE Chefia \');</script>';
        echo("<script>window.location.href = 'gestaopessoa.php?modulo=inicio&acao=C';</script>");
    } else {
        $_SESSION['cpfavaliado'] = $usucpf;
        $_SESSION['boautoavaliacao'] = true;
        header("Location: ?modulo=principal/formularioAvaliacao&acao=A");
    }
}

function verificaPontuacao($defid, $sercpf, $tavid) {
    global $db;

    $sql = "
        SELECT ROUND(AVG(resnota))
        FROM gestaopessoa.respostaavaliacao
        WHERE  defid = {$defid} AND sercpf = '{$sercpf}' AND tavid = {$tavid} AND resano = {$_SESSION['exercicio']}
    ";
    $valor = $db->pegaUm($sql);
    return $valor;
}

function arrayPerfil() {
    global $db;
    $sql = sprintf("
        SELECT
            pu.pflcod
           FROM
            seguranca.perfilusuario pu
            INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
                   p.sisid = 64
           WHERE
            pu.usucpf = '%s'
           ORDER BY
            p.pflnivel", $_SESSION['usucpf']
    );
    return (array) $db->carregarColuna($sql, 'pflcod');
}

function controlaPermissao($tipo) {
    $perfis = arrayPerfil();

    switch ($tipo) {
        case 'lista_completa':
            if (!in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfis) &&
                !in_array(PERFIL_SUPER_USER, $perfis) &&
                !in_array(PERFIL_AVAL_SERV_CONSULTA, $perfis)) {
                return false;
            } else {
                return true;
            }
            break;
        case 'consulta':
            if (in_array(PERFIL_AVAL_SERV_CONSULTA, $perfis) && count($perfis == 1)) {
                return true;
            } else {
                return false;
            }
            break;
        case 'superuser':
            if (in_array(PERFIL_SUPER_USER, $perfis) && count($perfis == 1)) {
                return true;
            } else {
                return false;
            }
            break;
        case 'administrador':
            if (in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfis) && count($perfis == 1)) {
                return true;
            } else {
                return false;
            }
            break;
        case 'avaliacao':
            if (in_array(PERFIL_AVAL_SERV_AVALIACAO, $perfis) && count($perfis == 1)) {
                return true;
            } else {
                return false;
            }
            break;
    }
}

function soConsulta() {
    global $db;
    $sql = "SELECT sercpfchefe FROM gestaopessoa.servidor WHERE sercpf = '" . $_SESSION['cpfavaliado'] . "'";
    $cpf = $db->pegaUm($sql);
    if (($cpf == $_SESSION['usucpf'])) {
        return false;
    } else {
        return true;
    }
}

function getQuantidade($tipo) {
    global $db;

    if ($tipo) {
        if ($tipo == TIPO_AVAL_CONSENSO) {
            $and = " AND s.sermediaconsenso = 'f' ";
        }
        $sql = "
            SELECT coalesce( count(distinct(s.sercpf)), 0)
            FROM gestaopessoa.servidor AS s
            WHERE s.sercpf IN (
            SELECT sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE tavid = $tipo AND resavaliacaopendente = 'f' AND resano = {$_SESSION['exercicio']}
            ) $and AND seranoreferencia = {$_SESSION['exercicio']}
        ";
        $valor = $db->pegaUm($sql);
        echo $valor;
    }
}

function getQtdMedia() {
    global $db;

    $sql = "
        SELECT coalesce( count(distinct(s.sercpf)), 0)
        FROM gestaopessoa.servidor AS s
        WHERE s.sermediaconsenso = 't'
    ";
    $valor = $db->pegaUm($sql);
    echo $valor;
}

function qtdServidores($cadastrados = FALSE) {
    global $db;

    if ($cadastrados) {
        $sql = "
            SELECT  count( distinct(s.sercpf) ) AS qtd_total
            FROM gestaopessoa.servidor AS s
            INNER JOIN seguranca.usuario AS u ON s.sercpf = u.usucpf
            INNER JOIN seguranca.usuario_sistema AS us ON us.usucpf = u.usucpf
            WHERE u.usucpf IS NOT NULL
            AND us.sisid = 64
            AND s.tssid IN (" . SITUACAO_ATIVO_PERMANENTE . "," . SITUACAO_CEDIDO . "," . SITUACAO_EXCEDENTE . "," . SITUACAO_ATIVO_PERM_L . "," . SITUACAO_ANISTIADO . "," . SITUACAO_EXERC . ")
            AND seranoreferencia = {$_SESSION['exercicio']}
        ";
    } else {
        $sql = "
            SELECT  count(distinct(s.sercpf)) AS qtd_total
            FROM gestaopessoa.servidor AS s
            WHERE s.tssid IN (" . SITUACAO_ATIVO_PERMANENTE . "," . SITUACAO_CEDIDO . "," . SITUACAO_EXCEDENTE . "," . SITUACAO_ATIVO_PERM_L . "," . SITUACAO_ANISTIADO . "," . SITUACAO_EXERC . ")
                AND seranoreferencia = {$_SESSION['exercicio']}
        ";
    }
    $qtd = $db->pegaUm($sql);
    echo $qtd;
}

function getSituacaoMEC($cpf) {
    global $db;
    $sql = "SELECT fstid FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '{$cpf}' ";
    $tipo = $db->pegaUm($sql);
    if ($tipo) {
        return $tipo;
    }
}

function controlaDadoFuncional($tipo) {
    global $db;
    include_once( APPRAIZ . "gestaopessoa/classes/FtDadoFuncional.class.inc" );
    $df = new FtDadoFuncional();
    switch ($tipo) {
        case VINCULO_EFETIVO:
            return $df->arEfetivo;
            break;
        case VINCULO_CEDIDO:
            return $df->arCedido;
            break;
        case VINCULO_CTU:
            return $df->arCTU;
            break;
        case VINCULO_CONSULTOR:
            return $df->arConsultor;
            break;
        case VINCULO_EXERCICIODES:
            return $df->arExercicioDes;
            break;
        case VINCULO_EXERCICIOPRO:
            return $df->arExercicioPro;
            break;
        case VINCULO_TERCEIRIZADO:
            return $df->arTerceirizado;
            break;
        case VINCULO_ANISTIADO_CLT:
            return $df->arAnistiadoCLT;
            break;
        case VINCULO_CARGOCOMISSIONADO:
            return $df->arCargoComissionado;
            break;
        case VINCULO_REQUISITADO:
            return $df->arRequisitados;
            break;
        case VINCULO_COLABORACAO_TECNICA:
            return $df->arColaboracaoTecnica;
            break;
        case VINCULO_EXAMINADOR_EXTERNO:
            return $df->arExaminadorexterno;
            break;
    }
}

function controlaPefilFT($operacao) {
    $arPerfis = arrayPerfil();
    /*
     * 	define( "PERFIL_FT_CONSULTA_GERAL",334);
      define( "PERFIL_FT_ADMINISTRADOR_GERAL",335);
      define( "PERFIL_FT_ADMINISTRADOR_CONTRATO",336);
      define( "PERFIL_FT_ADMINISTRADOR_PESSOAL",337);
      define( "PERFIL_FT_ADMINISTRADOR_PROJETO",338);
      define( "PERFIL_FT_FISCAL_CONTRATO",339);
      define( "PERFIL_FT_FISCAL_PESSOAL",340);
      define( "PERFIL_FT_FISCAL_PROJETO",341);

      define( "PERFIL_SERVIDOR",397);
      define( "PERFIL_CONSULTOR",398);
      define( "PERFIL_TERCEIRIZADO",399);
     */
    switch ($operacao) {
        case 'soConsulta':
            if (in_array(PERFIL_FT_CONSULTA_GERAL, $arPerfis) && count($arPerfis == 0)) {
                return true;
            } else {
                return false;
            }
            break;
        case 'permissaoTotal':
            if (in_array(PERFIL_SUPER_USER, $arPerfis) || in_array(PERFIL_FT_ADMINISTRADOR_GERAL, $arPerfis) || in_array(PERFIL_SUPER_USER, $arPerfis)) {
                return true;
            } else {
                return false;
            }
        case 'vinculosPermitidos':
            if (in_array(PERFIL_SUPER_USER, $arPerfis)) {
                $arPermitidos = array();
                array_push($arPermitidos, VINCULO_EFETIVO, VINCULO_CEDIDO, VINCULO_CTU, VINCULO_CONSULTOR, VINCULO_EXERCICIODES, VINCULO_EXERCICIOPRO, VINCULO_TERCEIRIZADO, VINCULO_ANISTIADO_CLT, VINCULO_CARGOCOMISSIONADO, VINCULO_REQUISITADO, VINCULO_COLABORACAO_TECNICA);
            } elseif (in_array(PERFIL_FT_FISCAL_PESSOAL, $arPerfis) || in_array(PERFIL_SERVIDOR, $arPerfis)) {
                $arPermitidos = array();
                array_push($arPermitidos, VINCULO_EFETIVO, VINCULO_CEDIDO, VINCULO_CTU, VINCULO_EXERCICIODES, VINCULO_EXERCICIOPRO, VINCULO_ANISTIADO_CLT, VINCULO_CARGOCOMISSIONADO, VINCULO_REQUISITADO, VINCULO_COLABORACAO_TECNICA);
            } elseif (in_array(PERFIL_FT_FISCAL_CONTRATO, $arPerfis) || in_array(PERFIL_TERCEIRIZADO, $arPerfis)) {
                $arPermitidos = array();
                array_push($arPermitidos, VINCULO_TERCEIRIZADO);
            } elseif (in_array(PERFIL_FT_FISCAL_PROJETO, $arPerfis) || in_array(PERFIL_CONSULTOR, $arPerfis)) {
                $arPermitidos = array();
                array_push($arPermitidos, VINCULO_CONSULTOR);
            }

            return $arPermitidos;
    }
}

function prazoVencido() {

    include_once APPRAIZ . "includes/classes/dateTime.inc";

    $data = new Data();
    $agora = $data->timeStampDeUmaData(date("d/m/Y"));
    $limite = $data->timeStampDeUmaData( DATA_AVALIAÇÃO_FINALIZADA );

    if ($agora > $limite){
        return true;
    } else {
        return false;
    }
}

function verificaMediaConsenso() {
    global $db;
    $boSql = "SELECT sermediaconsenso FROM gestaopessoa.servidor WHERE
                         sercpf = '" . $_SESSION['cpfavaliado'] . "'";
    //	dbg($boSql);
    if ($boMedia = $db->pegaUm($boSql)) {
        if ($boMedia == 't') {
            return false;
        }
        return true;
    }
    return true;
}


#funcão modificada - usada em outro lugar!
function direcionaFT() {
    global $db;

    $arPerfis = arrayPerfil();

    if ((in_array(PERFIL_SERVIDOR, $arPerfis) || in_array(PERFIL_CONSULTOR, $arPerfis) || in_array(PERFIL_TERCEIRIZADO, $arPerfis)) && !in_array(PERFIL_FT_ADMINISTRADOR_GERAL, $arPerfis)) {

        unset($_SESSION['fdpcpf']);
        $fdpCpf = $db->pegaUm("select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '" . $_SESSION['usucpf'] . "'");
        if ($fdpCpf) {
            include_once( APPRAIZ . "gestaopessoa/classes/FtDadoPessoal.class.inc" );
            $ft = new FtDadoPessoal();
            $ft->carregarPorId("'" . $fdpCpf . "'");
            $sql = "SELECT * FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '" . $fdpCpf . "'";
            $dados = $db->carregar($sql);
            $_SESSION['fdpcpf'] = $fdpCpf;
        }
        header("Location: ?modulo=principal/cadDadosPessoais&acao=A");
    } else {
        if (!in_array(PERFIL_SERVIDOR, $arPerfis) & !in_array(PERFIL_CONSULTOR, $arPerfis) & !in_array(PERFIL_SUPER_USER, $arPerfis) & !in_array(PERFIL_TERCEIRIZADO, $arPerfis) & !in_array(PERFIL_FT_ADMINISTRADOR_GERAL, $arPerfis) & !in_array(PERFIL_FT_CONSULTA_GERAL, $arPerfis)) {
        ?>
            <script> alert('Acesso negado. Usuário sem perfil cadastrado no sistema.'); </script>
            <script> window.location.href = '?modulo=inicio&acao=C'; </script>
        <?
            exit;
        }
    }
}

function bloqueiaEdicaoFT() {

    $perfis = arrayPerfil();

    if (!in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfis) && !in_array(PERFIL_SUPER_USER, $perfis) && !in_array(PERFIL_AVAL_SERV_CONSULTA, $perfis) && !in_array(PERFIL_AVAL_SERV_AVALIACAO, $perfis) && !in_array(PERFIL_TERCEIRIZADO, $perfis) && !in_array(PERFIL_SERVIDOR, $perfis)) {
        $resultado = "disabled=disabled";
    } else {
        $resultado = "";
    }
    return $resultado;
}

function ft_monta_sql_relatorio() {

    $where = array();

    extract($_REQUEST);

    // Situação no MEC
    if ($fstid[0] && $fstid_campo_flag) {
        array_push($where, " st.fstid " . (!$fstid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $fstid) . "') ");
    }

    // Estado Civil
    if ($eciid[0] && $eciid_campo_flag) {
        array_push($where, " dp.eciid " . (!$eciid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $eciid) . "') ");
    }

    // Sexo
    if ($fdpsexo) {
        array_push($where, " dp.fdpsexo IN ('" . implode("','", $fdpsexo) . "') ");
    }

    // Data de Nascimento
    if ($dtnascinicio && !$dtnascfim) {
        $dtnascinicio = explode("/", $dtnascinicio);
        $dtnascinicio = $dtnascinicio[2] . "-" . $dtnascinicio[1] . "-" . $dtnascinicio[0];
        array_push($where, " us.usudatanascimento = '" . $dtnascinicio . "' ");
    } elseif ($dtnascinicio && $dtnascfim) {
        $dtnascinicio = explode("/", $dtnascinicio);
        $dtnascinicio = $dtnascinicio[2] . "-" . $dtnascinicio[1] . "-" . $dtnascinicio[0];
        $dtnascfim = explode("/", $dtnascfim);
        $dtnascfim = $dtnascfim[2] . "-" . $dtnascfim[1] . "-" . $dtnascfim[0];
        array_push($where, " us.usudatanascimento >= '" . $dtnascinicio . "' AND us.usudatanascimento <= '" . $dtnascfim . "' ");
    }

    // UF
    if ($estuf[0] && $estuf_campo_flag) {
        array_push($where, " dp.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $estuf) . "') ");
    }

    // Grupo Sanguineo
    if ($fdpgruposanguineo) {
        array_push($where, " dp.fdpgruposanguineo IN ('" . implode("','", $fdpgruposanguineo) . "') ");
    }

    // Fator RH
    if ($fdpfatorrh) {
        array_push($where, " dp.fdpfatorrh IN ('" . implode("','", $fdpfatorrh) . "') ");
    }

    // Pessoa com Deficiência
    if ($fdpdeficiente) {
        array_push($where, " dp.fdpdeficiente IN ('" . implode("','", $fdpdeficiente) . "') ");
    }

    // Tipo de Deficiência
    if ($fdpdeficiencia[0] && $fdpdeficiencia_campo_flag) {
        array_push($where, " dp.fdpdeficiencia " . (!$fdpdeficiencia_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $fdpdeficiencia) . "') ");
    }

    // Cargo Efetivo no MEC
    if ($fcmid[0] && $fcmid_campo_flag) {
        array_push($where, " df.fcmid " . (!$fcmid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $fcmid) . "') ");
    }

    // Exerce Cargo ou Função
    if ($fdpexercecargofuncao) {
        array_push($where, " df.fdfexercecargofuncao IN ('" . implode("','", $fdpexercecargofuncao) . "') ");
    }

    // Unidade de Lotação
    if ($fulid[0] && $fulid_campo_flag) {
        array_push($where, " df.fulid " . (!$fulid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $fulid) . "') ");
    }

    // Grau de Escolaridade
    if ($tfoid[0] && $tfoid_campo_flag) {
        array_push($where, " fa.tfoid " . (!$tfoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $tfoid) . "') ");
    }

    // Situação do Curso
    if ($ffasituacao) {
        array_push($where, " fa.ffasituacao = '" . $ffasituacao . "' ");
    }

    // Ano de Conclusão do Curso
    if ($ffaanoconclusao) {
        array_push($where, " fa.ffaanoconclusao = '" . $ffaanoconclusao . "' ");
    }

    // Idioma
    if ($ftiid[0] && $ftiid_campo_flag) {
        array_push($where, " id.ftiid " . (!$ftiid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $ftiid) . "') ");
    }

    // Conceito Idioma
    if ($ftcidleitura) {
        array_push($where, " id.ftcidleitura IN ('" . implode("','", $ftcidleitura) . "') ");
    }
    if ($ftcidfala) {
        array_push($where, " id.ftcidfala IN ('" . implode("','", $ftcidfala) . "') ");
    }
    if ($ftcidescrita) {
        array_push($where, " id.ftcidescrita IN ('" . implode("','", $ftcidescrita) . "') ");
    }

    // Atividade Desenvolvida
    if ($ftaid[0] && $ftaid_campo_flag) {
        array_push($where, " ad.ftaid " . (!$ftaid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $ftaid) . "') ");
    }

    // Nível de atividade desenvolvida
    if ($fnaid) {
        array_push($where, " ad.fnaid IN ('" . implode("','", $fnaid) . "') ");
    }

    // Tipo de Experiência Anterior
    if ($fteid[0] && $fteid_campo_flag) {
        array_push($where, " ea.fteid " . (!$fteid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $fteid) . "') ");
    }

    // MONTA SQL
    $sql = "
        SELECT  UPPER(fdpnome) as nomedapessoa,
                fdpnome as nomedapessoaxls,
                fstdescricao as fstid,
                fuldescricao as fulid,
                estuf,
                df.fcmid as fcmid,
                tf.tfodsc as tfoid,
                ti.ftidescricao as ftiid,
                ta.ftadescricao as ftaid,
                te.ftedescricao as fteid,
                INITCAP(dp.fdpdeficiencia) as fdpdeficiencia,
                INITCAP(fa.ffacurso) as ffacurso

        FROM gestaopessoa.ftdadopessoal dp

        INNER JOIN gestaopessoa.ftdadofuncional df 				ON df.fdpcpf = dp.fdpcpf
        INNER JOIN gestaopessoa.ftformacaoacademica fa			ON fa.fdpcpf = dp.fdpcpf
        INNER JOIN gestaopessoa.idioma id						ON id.fdpcpf = dp.fdpcpf
        INNER JOIN gestaopessoa.ftatividadedesenvolvida ad		ON ad.fdpcpf = dp.fdpcpf
        INNER JOIN gestaopessoa.ftexperienciaanterior ea		ON ea.fdpcpf = dp.fdpcpf
        INNER JOIN gestaopessoa.ftsituacaotrabalhador st 		ON st.fstid = dp.fstid
        LEFT JOIN gestaopessoa.ftcargoefetivomec ce 			ON ce.fcmid = df.fcmid
        INNER JOIN gestaopessoa.ftunidadelotacao ul				ON ul.fulid = df.fulid
        INNER JOIN gestaopessoa.fttipoatividadedesenvolvida ta	ON ta.ftaid = ad.ftaid
        INNER JOIN gestaopessoa.ftitipoidioma ti				ON ti.ftiid = id.ftiid
        INNER JOIN gestaopessoa.fttipoexperienciaanterior te	ON te.fteid = ea.fteid
        INNER JOIN seguranca.usuario us							ON us.usucpf = dp.fdpcpf
        INNER JOIN public.tipoformacao tf						ON tf.tfoid = fa.tfoid

        /*
        INNER JOIN public.estadocivil ec 						ON ec.eciid = dp.eciid --Pode sair
        INNER JOIN territorios.estado et 						ON et.estuf = dp.estuf --Pode sair
        */

        " . ( is_array($where) ? ' WHERE' . implode(' AND ', $where) : '' ) . $stFiltro . "

        GROUP BY nomedapessoa, nomedapessoaxls, st.fstdescricao, ul.fuldescricao, dp.estuf, df.fcmid, df.fulid, fa.tfoid,
                id.ftiid, ad.ftaid, ea.fteid, ta.ftadescricao, ti.ftidescricao, tf.tfodsc, te.ftedescricao, dp.fdpdeficiencia,
                fa.ffacurso

        ORDER BY " . (is_array($agrupador) ? implode(",", $agrupador) : "pais");

    //	ver($sql, $_REQUEST);
    //	die();
    return $sql;
}

function ft_monta_agp_relatorio() {

    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

    $agp = array(
        "agrupador" => array(),
        "agrupadoColuna" => array("fstid",
            "fulid",
            "tfoid",
            "fdpdeficiencia",
            "ffacurso"),
    );


    foreach ($agrupador as $val) {
        switch ($val) {
            case "fstid":
                array_push($agp['agrupador'], array(
                    "campo" => "fstid",
                    "label" => "Situação no MEC")
                );
                break;
            case "estuf":
                array_push($agp['agrupador'], array(
                    "campo" => "estuf",
                    "label" => "UF")
                );
                break;
            case "fcmid":
                array_push($agp['agrupador'], array(
                    "campo" => "fcmid",
                    "label" => "Cargo Efetivo no MEC")
                );
                break;
            case "fulid":
                array_push($agp['agrupador'], array(
                    "campo" => "fulid",
                    "label" => "Unidade de Lotação")
                );
                break;
            case "tfoid":
                array_push($agp['agrupador'], array(
                    "campo" => "tfoid",
                    "label" => "Grau de Escolaridade")
                );
                break;
            case "ftiid":
                array_push($agp['agrupador'], array(
                    "campo" => "ftiid",
                    "label" => "Idioma")
                );
                break;

            case "ftaid":
                array_push($agp['agrupador'], array(
                    "campo" => "ftaid",
                    "label" => "Atividade Desenvolvida")
                );
                break;
            /*
              case "fdpdeficiencia":
              array_push($agp['agrupador'], array(
              "campo" => "fdpdeficiencia",
              "label" => "Deficiência")
              );
              break;
             */
            case "fteid":
                array_push($agp['agrupador'], array(
                    "campo" => "fteid",
                    "label" => "Tipo de Experiência Anterior")
                );
                break;

            case "nomedapessoa":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedapessoa",
                    "label" => "Nome da Pessoa")
                );
                break;
            case "nomedapessoaxls":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedapessoaxls",
                    "label" => "Nome da Pessoa")
                );
                break;
            case "nivelpreenchimento":
                array_push($agp['agrupador'], array(
                    "campo" => "nivelpreenchimento",
                    "label" => "Nível de Preenchimento")
                );
                break;
        }
    }

    array_push($agp['agrupador'], array(
        "campo" => "nomedapessoa",
        "label" => "Nome da Pessoa")
    );


    return $agp;
}

function ft_monta_coluna_relatorio() {

    global $_REQUEST;

    $coluna = array();

    /* foreach ( $_REQUEST['modalidade'] as $valor ){

      switch( $valor ){

      case 'M':
      array_push( $coluna, array("campo" 	  => "medio",
      "label" 	  => "Ensino Médio",
      "blockAgp" => "nomedaescola",
      "type"	  => "character") );
      break;
      case 'F':
      array_push( $coluna, array("campo" 	  => "fundamental",
      "label" 	  => "Ensino Fundamental",
      "blockAgp" => "nomedaescola",
      "type"	  => "character") );
      break;
      }

      } */

    array_push($coluna, array("campo" => "fstid",
        "label" => "Situação no MEC",
        //"blockAgp" 	=> "nomedapessoa",
        "type" => "character"));

    array_push($coluna, array("campo" => "fulid",
        "label" => "Unidade de Lotação",
        //"blockAgp" 	=> "nomedapessoa",
        "type" => "character"));

    array_push($coluna, array("campo" => "tfoid",
        "label" => "Formação",
        //"blockAgp" 	=> "nomedapessoa",
        "type" => "character"));

    array_push($coluna, array("campo" => "ffacurso",
        "label" => "Curso",
        //"blockAgp" 	=> "nomedapessoa",
        "type" => "character"));

    if ($_REQUEST['fdpdeficiente'][0] != '') {
        foreach ($_REQUEST['fdpdeficiente'] as $dados) {

            if ($dados['fdpdeficiente'] == 't') {
                array_push($coluna, array("campo" => "fdpdeficiencia",
                    "label" => "Deficiência",
                    //"blockAgp" 	=> "nomedapessoa",
                    "type" => "character"));
            }
        }
    }

    return $coluna;
}

function salvarOcupacao() {
    global $db;

    extract($_POST);

    $fdpcpf = "'{$_SESSION['fdpcpf']}'";

    if (!$fdpcpf) {
        return array("msg" => "Não foi possível realizar a operação!");
    }

    if (!$ocpcargo || !$ocplotacao || !$ocpdtingresso) {
        return array("msg" => "Favor preencher todos os campos obrigatórios!");
    }

    $ocpcargo = "'$ocpcargo'";
    $ocpcodcargo = "'$ocpcodcargo'";
    $ocpvalor = str_replace(array(".", ","), array("", "."), $ocpvalor);
    $ocplotacao = "'$ocplotacao'";
    $ocpdtingresso = "'" . formataDataBanco($ocpdtingresso) . "'";
    $ocpdtdesligamento = $ocpdtdesligamento ? "'" . formataDataBanco($ocpdtdesligamento) . "'" : "NULL";
    $ocpindpolitica = "'$ocpindpolitica'";
    $ocppartido = "'$ocppartido'";
    $ocpobs = "'$ocpobs'";
    $ocpforcapolitica = "'$ocpforcapolitica'";

    if ($ocpid) {
        $sql = "update
                                        gestaopessoa.ocupacao
                                set
                                        ocpcargo = $ocpcargo,
                                        ocpcodcargo = $ocpcodcargo,
                                        ocpvalor = $ocpvalor,
                                        ocplotacao = $ocplotacao,
                                        ocpdtingresso = $ocpdtingresso,
                                        ocpdtdesligamento = $ocpdtdesligamento,
                                        ocpindpolitica = $ocpindpolitica,
                                        ocppartido = $ocppartido,
                                        ocpobs = $ocpobs,
                                        ocpforcapolitica = $ocpforcapolitica
                                where
                                        ocpid = $ocpid
                                and
                                        fdpcpf = $fdpcpf";
        $db->executar($sql);
    } else {
        $sql = "insert into
                                        gestaopessoa.ocupacao
                                (fdpcpf,ocpcargo,ocpcodcargo,ocpvalor,ocplotacao,ocpdtingresso,ocpdtdesligamento,ocpindpolitica,ocppartido,ocpobs,ocpforcapolitica)
                                        values
                                ($fdpcpf,$ocpcargo,$ocpcodcargo,$ocpvalor,$ocplotacao,$ocpdtingresso,$ocpdtdesligamento,$ocpindpolitica,$ocppartido,$ocpobs,$ocpforcapolitica)
                                        returning ocpid";
        $ocpid = $db->pegaUm($sql);
    }

    $sqlR = "delete from gestaopessoa.relacionamentoocupacao where ocpid = $ocpid;";
    if ($arrFdpcpf) {
        $i = 0;
        foreach ($arrFdpcpf as $cpf) {
            $sqlR .= "	insert into
                                                        gestaopessoa.relacionamentoocupacao
                                                (fdpcpf,ocpid,rloobs)
                                                        values
                                                ('$cpf',$ocpid,'{$rloobs[$i]}');";
            $i++;
        }
    }
    $db->executar($sqlR);

    if ($db->commit()) {
        return array("msg" => "Operação realizada com sucesso!");
    } else {
        return array("msg" => "Não foi possível realizar a operação!");
    }
}

    function buscarDadosServidorCedido( $fdpcpf ){
        global $db;

        $sql = "
            SELECT  fdpcargodas,
                    fdptipodas,
                    fdplocaltrabalho,
                    fdpfonelocaltrabalho,
                    fdpchefiaimediataloctrab,
                    fdpemailchefiaimediata,
                    fdpfonechefiaimediata
            FROM gestaopessoa.ftdadopessoal
            WHERE fdpcpf = '{$fdpcpf}';
        ";
        return $db->pegaLinha($sql);
    }


    function salvarDadosServCedidos( $dados ){
        global $db;

        $fdpcpf                     = simec_trim($dados['fdpcpf']);
        $fdpcargodas                = simec_trim($dados['fdpcargodas']) == 'S' ? 't' : 'f';
        $fdptipodas                 = simec_trim($dados['fdptipodas']) == '' ? 'NULL' : "'".simec_trim($dados['fdptipodas'])."'";
        $fdplocaltrabalho           = simec_trim($dados['fdplocaltrabalho']);
        $fdpfonelocaltrabalho       = simec_trim($dados['fdpfonelocaltrabalho']);
        $fdpchefiaimediataloctrab   = simec_trim($dados['fdpchefiaimediataloctrab']);
        $fdpemailchefiaimediata     = simec_trim($dados['fdpemailchefiaimediata']);
        $fdpfonechefiaimediata      = simec_trim($dados['fdpfonechefiaimediata']);

        if( $fdpcpf != '' ){
            $sql = "
                UPDATE gestaopessoa.ftdadopessoal
                    SET fdpcargodas                = '$fdpcargodas',
                        fdptipodas                 = {$fdptipodas},
                        fdplocaltrabalho           = '{$fdplocaltrabalho}',
                        fdpfonelocaltrabalho       = '{$fdpfonelocaltrabalho}',
                        fdpchefiaimediataloctrab   = '{$fdpchefiaimediataloctrab}',
                        fdpemailchefiaimediata     = '{$fdpemailchefiaimediata}',
                        fdpfonechefiaimediata      = '{$fdpfonechefiaimediata}'
                WHERE fdpcpf = '{$fdpcpf}' RETURNING fdpcpf;
            ";
            $result = $db->pegaUm($sql);
        }

        if( $result > 0 ){
            $db->commit();
            $db->sucesso( 'principal/forca_trabalho/cad_dados_servidores_cedidos','', 'Operação realizada com sucesso!');
        }else{
            $db-rollback();
            $db->sucesso( 'principal/forca_trabalho/cad_dados_servidores_cedidos','', 'Ocorreu um erro, tente novamente mais tarde!');
        }
    }


#FORMATA A DATA PADRÃO BR PARA O PADRÃO AMERICANO (PREPARA PARA O BANCO DE DADOS).

function formataDataBanco($valor) {
    $data = explode("/", $valor);
    $dia = $data[0];
    $mes = $data[1];
    $ano = $data[2];
    return $ano . "-" . $mes . "-" . $dia;
}

//refeita - novo
function verificaDadosPesFunc($cpf) {
    global $db;
    $sql_pes = "SELECT fdpcpf FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '$cpf'";
    $sql_func = "SELECT fdpcpf FROM gestaopessoa.ftdadofuncional WHERE fdpcpf = '$cpf'";
    $pes = $db->pegaUm($sql_pes);
    $func = $db->pegaUm($sql_func);
    if (!empty($pes) && !empty($func)) {
        echo 1;
    } else {
        echo 0;
    }
}

function verificaPerfilGestoreAidp() {
    global $db;
    global $filtroGestor;
    global $filtroAidp;
    global $msgPerfil;

    $perfil = arrayPerfil();

    //if ( !in_array(PERFIL_SUPER_USER, $perfil) ){
    if (in_array(PERFIL_GESTOR, $perfil)) {
        $filtroGestor = "";
        $sql = "SELECT
                                        f.fstid as codigo
                                FROM
                                        gestaopessoa.ftsituacaotrabalhador f
                                        INNER JOIN gestaopessoa.usuarioresponsabilidade ur ON ur.fstid = f.fstid
                                WHERE
                                        ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                        ur.pflcod = " . PERFIL_GESTOR . " AND
                                        ur.rpustatus = 'A'";
        $RS = $db->carregarColuna($sql);

        if ($RS) {
            $filtroGestor = implode(",", $RS);
        }

        if (!$filtroGestor)
            $msgPerfil = "ERRO: Perfil Gestor não configurado corretamente. Por favor, entre em contato com o Gestor do sistema para corrigir o problema.";
    }
    if (in_array(PERFIL_AIDP, $perfil)) {
        $filtroAidp = "";

        $sql = "
                        SELECT f.fulid AS codigo
                        FROM gestaopessoa.ftunidadelotacao f
                        INNER JOIN gestaopessoa.usuarioresponsabilidade ur ON ur.fulid = f.fulid
                        WHERE ur.usucpf = '" . $_SESSION['usucpf'] . "' AND ur.pflcod = " . PERFIL_AIDP . " AND ur.rpustatus = 'A'
                        ";
        $RS = $db->carregar($sql);

        if ($RS) {
            $filtroAidp = implode(",", $RS);
        }

        if (!$filtroAidp)
            $msgPerfil = "ERRO: Perfil AIDP não configurado corretamente. Por favor, entre em contato com o Gestor do sistema para corrigir o problema.";
    }
}

#------------------------------------------------------------- FUNÇÕES MODULO AVALIAÇÃO DE SERVIDOR --------------------------------------------------#
#AS FUNÇÕES SÃO: (EM ORDER ALFABETICA)
# - atualizaServidorEquipeSimNao;
# - atualizaStatusLiberaParecer;
# - buscaDadosServidorAvaliacao;
# - carregaListaEquipe;
# - deletarFecheEquipe;
# - deletarServidorEquipe;
# - existeAvaliacaoTipo;
# - salvarFecheEquipe;
# - salvarServidorEquipe;
# - verificaDadosServidorAtualizados;
# - verificarTipoAvaliacao;
# - verificaSeHaResposta;
# - tratamentoMembrosEquipe;
# - quantidadeServidorAvaliados;
# - quantidadeServidorCadastrados;


# - atribuiCPFSessao: TELA LISTAGEM DOS SERVIDORES - CRIA A SESSÃO fdpcpf, USADO PARA EDITAR OS DADOS NA TELA DE CADASTRO DO FORÇA DE TRABALHO.
function atribuiCPFSessao( $dados ){
    $cpf = $dados['fdpcpf'];

    if( $cpf != '' ){
        $_SESSION['fdpcpf'] = $cpf;
        echo '<resp>OK</resp>';
        die();
    }
}

# - atualizaServidorEquipeSimNao: TELA LISTAGEM DOS SERVIDORES - ATUALIZA O DADO DO SERVIDOR, SE O RESPECTIVO FAZ OU NÃO PARTE DE UMA EQUIPE.
function atualizaServidorEquipeSimNao($dados) {
    global $db;

    $exc_aval   = $dados['exc_aval'];
    $sercpf     = $dados['cpf'];

    if( $exc_aval == 'S' && $sercpf != '' ){
        #DELETA AS AVALIAÇOES FEITAS POR ELE E A ELE.
        $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and sercpf = '{$sercpf}';";
        $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and resavaliacpf = '{$sercpf}' RETURNING resid;";
        $db->pegaUm($sql);
    }

    $sql = "
        UPDATE gestaopessoa.servidor
            SET sercomequipe = {$dados['status']}
        WHERE sercpf = '{$dados['cpf']}' AND seranoreferencia = {$_SESSION['exercicio']}
        RETURNING sercpf;
    ";
    $dado = $db->pegaLinha($sql);

    if( $dado['sercpf'] > 0 ){
        $db->commit();

        if($dados['status'] == 'true'){
            echo "<resp> <div id=\"servidor_{$dados['cpf']}\"> <input type=\"checkbox\" id=\"servidor_{$dados['cpf']}\" name=\"servidor\" value=\"{$dados['cpf']}\" onclick=\"atualizaServidorEquipeSimNao(this);\" checked=\"checked\"> <br> <span style=\"color:green\"> Com Equipe </span> </div> </resp>";
        }elseif($dados['status'] == 'false'){
            echo "<resp> <div id=\"servidor_{$dados['cpf']}\"> <input type=\"checkbox\" id=\"servidor_{$dados['cpf']}\" name=\"servidor\" value=\"{$dados['cpf']}\" onclick=\"atualizaServidorEquipeSimNao(this);\"> <br> <span style=\"color:red\"> Sem Equipe </span> </div> </resp>";
        }
        die();
    }
}

# - atualizaStatusLiberaParecer: TELA PEDIDO DE RECONCIDERAÇÃO - DEFINE SE É POSSIVEL REALIZAR O PARECER DO FECHE.
function atualizaStatusLiberaParecer( $dados ) {
	global $db;

	$sql = "
        UPDATE gestaopessoa.ftpedidoreconsideracao
            SET ftprliberaparecerchefe = {$dados['status']}
        WHERE ftprid = {$dados['ftprid']}
        RETURNING ftprid;
    ";
	$dado = $db->pegaLinha($sql);

    if($dado['ftprid'] > 1){
        $db->commit();

        if($dados['status'] == 'true'){
            echo "
                <resp>
                    <input type=\"checkbox\" checked=\"checked\" name=\"ftprliberaparecerchefe\" id=\"ftprliberaparecerchefe\" value=\"S\" onclick=\"atualizaStatusLiberaParecer(this);\">
                    <span style=\"position:relative; top:-4px; color:green;\">Liberar pedido de Análise da Chefia</span>
                </resp>
            ";
        }elseif($dados['status'] == 'false'){
            echo "
                <resp>
                    <input type=\"checkbox\" name=\"ftprliberaparecerchefe\" id=\"ftprliberaparecerchefe\" value=\"S\" onclick=\"atualizaStatusLiberaParecer(this);\">
                    <span style=\"position:relative; top:-4px;\">Liberar pedido de Análise da Chefia</span>
                </resp>
            ";
        }
        die();
    }
}

# - buscaDadosServidorAvaliacao: TELA FORMULARIO DE AVALIAÇÃO - BUSCA DADOS DO SERVIDOR PARA PREENCHIMENTO DO "CABEÇALHO".
function buscaDadosServidorAvaliacao( $sercpf ){
    global $db;

	$sql = "
        SELECT  s.sernome,
                s.sersiape,
                s.sercargo,
                s.sercpfchefe,
                cf.sernome AS serdescricao_chefe,
                s.sergratificacao
        FROM gestaopessoa.servidor AS s

        LEFT JOIN(
            SELECT sercpf, sernome FROM gestaopessoa.servidor
        ) AS cf ON cf.sercpf = s.sercpfchefe

        WHERE s.sercpf = '{$sercpf}' AND s.seranoreferencia = {$_SESSION['exercicio']}
    ";
	$dados = $db->pegaLinha( $sql );
	return $dados;
}

# - buscaPermissaoReconcideracao: TELA PEDIDO DE RECONCIDERAÇÃO - BUSCA A PERMISSÃO DO USUÁRIO, SE ELE PODE TER ACESSO A TELA. "DEVE SER O CHEFE OU O PROPRIO" E SE O USUARIO LOGADO É O PROPRIO OU O SEU CHEFE.
function buscaPermissaoReconcideracao( $sercpf ){
    global $db;

    $perfil = pegaPerfilGeral();

    $sql = "
        SELECT  sercpf,
                sercpfchefe
        FROM gestaopessoa.servidor
        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}'
    ";
    $dados = $db->pegaLinha( $sql );

    $arrDados['sercpf']         = $dados['sercpf'];
    $arrDados['sercpfchefe']    = $dados['sercpfchefe'];

    if( ( $_SESSION['usucpf'] == $dados['sercpf'] ) || ( $_SESSION['usucpf'] == $dados['sercpfchefe'] ) || (in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil )) ){
        $arrDados['permissao'] = 'S';
    }else{
        $arrDados['permissao'] = 'N';
    }

    if( $dados['sercpf'] == $_SESSION['usucpf'] ){
        $arrDados['usuario_tipo'] = 'SU';

    }elseif( $dados['sercpfchefe'] == $_SESSION['usucpf'] ){
        $arrDados['usuario_tipo'] = 'CH';

    }else{
        $arrDados['usuario_tipo'] = 'NA';
    }

    return $arrDados;
}

# - buscarStatusLiberaParecer: TELA PEDIDO DE RECONCIDERAÇÃO - BUSCA O STATUS DO "REALIZAR O PARECER DO FECHE" CASO TRUE OU FALSE.
function buscaStatusLiberaParecer( $ftprid ){
    global $db;

    $sql = "
        SELECT ftprliberaparecerchefe
        FROM gestaopessoa.ftpedidoreconsideracao
        WHERE ftprid = {$ftprid}
    ";

    $dados = $db->pegaUm( $sql );
	return $dados;
}

# - cadastrarPessoa: TELA LISTAGEM DOS SERVIDORES - MATA A SESSÃO fdpcpf, USADO PARA CADASTRAR NOVOS USUÁRIOS DO FORÇA DE TRABALHO.
function cadastrarPessoa(){
    unset( $_SESSION['fdpcpf'] );
    echo '<resp>OK</resp>';
    die();
}

# - buscarStatusLiberaParecer: USADO NA TELA CADASTRAMENTO DA AVALIAÇÃO
function calculaNotaFinal( $sercpf ){
    global $db;

    $sql = "
        SELECT	sercpf,
                avaliacao_final,
                CASE
                    WHEN avaliacao_final = 0 THEN '0'
                    WHEN avaliacao_final > 0 AND avaliacao_final <= 30 THEN '6'
                    WHEN avaliacao_final >= 31 AND avaliacao_final <= 40 THEN '8'
                    WHEN avaliacao_final >= 41 AND avaliacao_final <= 50 THEN '10'
                    WHEN avaliacao_final >= 51 AND avaliacao_final <= 60 THEN '12'
                    WHEN avaliacao_final >= 61 AND avaliacao_final <= 70 THEN '14'
                    WHEN avaliacao_final >= 71 AND avaliacao_final <= 80 THEN '16'
                    WHEN avaliacao_final >= 81 AND avaliacao_final <= 90 THEN '18'
                    WHEN avaliacao_final >= 91 AND avaliacao_final <= 100 THEN '20'
                END AS pontos
        FROM (
            SELECT  s.sercpf,
                    CASE WHEN ( sp.soma_nota_sup IS NOT NULL AND e.soma_nota_eqp IS NOT NULL )
                            THEN ROUND( ( p.soma_nota_auto * 0.15 ) + ( sp.soma_nota_sup * 0.60) + ( e.soma_nota_eqp * 0.25) )
                        WHEN ( sp.soma_nota_sup IS NOT NULL AND p.soma_nota_auto IS NOT NULL )
                            THEN ROUND( ( p.soma_nota_auto * 0.275) + ( sp.soma_nota_sup * 0.725 ) )
                        WHEN ( sp.soma_nota_sup IS NOT NULL )
                            THEN ROUND( sp.soma_nota_sup )
                         ELSE 0
                    END AS avaliacao_final
            FROM gestaopessoa.respostaavaliacao r
            JOIN gestaopessoa.servidor s ON s.sercpf = r.sercpf

            LEFT JOIN(
                SELECT  sercpf,
			SUM( ROUND(nota) ) as soma_nota_auto
                FROM(
                    SELECT  s.sercpf,
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 1
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f
                GROUP BY sercpf
            ) AS p ON p.sercpf = s.sercpf

            LEFT JOIN(
                SELECT  sercpf,
			SUM( ROUND(nota) ) as soma_nota_sup
                FROM(
                    SELECT  s.sercpf,
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 2
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f
                GROUP BY sercpf
            ) AS sp ON sp.sercpf = s.sercpf

            LEFT JOIN(
                SELECT  sercpf,
			SUM( ROUND(nota) ) as soma_nota_eqp
                FROM(
                    SELECT  s.sercpf,
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 3
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f
                GROUP BY sercpf
            ) AS e ON e.sercpf = s.sercpf

            WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']}

            GROUP BY s.sersiape, s.sercpf, s.sernome, resavaliacaopendente, p.soma_nota_auto, sp.soma_nota_sup, soma_nota_eqp, s.sercomequipe

        ) AS AVALIACAO

        WHERE sercpf = '{$sercpf}'
        ";
    $nota_avaliacao = $db->pegaLinha($sql);

    return $nota_avaliacao;
}

# - carregaListaEquipe: TELA LISTAGEM DOS CHEFE/EQUIPES - CARREGA LISTAGEM DAS EQUIPE RELACIOANDA AO USUÁRIO SELECIONADO "AO CHEFE" DE UMA EQUIPE.
function carregaListaEquipe( $dados ){
    global $db;

    $sercpfchefe = $dados['sercpfchefe'];

    $acao = "
        <center>
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletarServidorEquipe(\''||s.sercpf||'\')\" title=\"Ritirar Servidor da Equipe\">
        </center>
    ";

    $equipe_sim = "
        <div id=\"servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"atualizaServidorEquipeSimNao(this);\" checked=\"checked\"> <br> <span style=\"color:green\"> Com Equipe </span> </div>
    ";

    $equipe_nao = "
        <div id=\"servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" id=\"servidor_'|| s.sercpf ||'\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"atualizaServidorEquipeSimNao(this);\"> <br> <span style=\"color:red\"> Sem Equipe </span> </div>
    ";

    $sql = "
        SELECT  '{$acao}' as acao,
                s.sersiape,
                '<span style=\"color:#1E90FF\">'||replace(to_char(cast(s.sercpf as bigint), '000:000:000-00'), ':', '.')||'</span>' as sercpf,
                s.sernome,

                CASE WHEN (s.sercargo = 'NULL' OR s.sercargo = '')
                    THEN '-'
                    ELSE s.sercargo
                END AS sercargo,

                CASE WHEN s.sercomequipe = TRUE
                    THEN '{$equipe_sim}'
                    ELSE '{$equipe_nao}'
                END AS sercomequipe,

                CASE WHEN s.tssid IS NULL
                    THEN '-'
                    ELSE t.tssdescricao
                END AS tssdescricao,

                CASE WHEN  (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) != 0
                    THEN 'Chefe'
                    ELSE 'Subordinado'
                END as hierarquia,

                CASE
                    WHEN sertipogratificacao = 'PE' THEN 'GDPGPE'
                    WHEN sertipogratificacao = 'CE' THEN 'GDACE'
                    WHEN sertipogratificacao = 'OS' THEN 'CEDIDO'
                    WHEN sertipogratificacao = 'PS' THEN 'GDAPS'
                    WHEN sertipogratificacao = 'FE' THEN 'Chefia Cedido'
                END AS sertipogratificacao,

                CASE WHEN a.tavid = 1
                    THEN '<img align=\"absmiddle\" src=\"/imagens/pd_normal.JPG\" title=\"Situação: Avaliação Realizada\" >'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/pd_urgente.JPG\" title=\"Situação: Avaliação Não Realizada\" >'
                END as auto,

                CASE WHEN b.tavid = 2
                    THEN '<img align=\"absmiddle\" src=\"/imagens/pd_normal.JPG\" title=\"Situação: Avaliação Realizada\" >'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/pd_urgente.JPG\" title=\"Situação: Avaliação Não Realizada\" >'
                END as chefe,

                --O SUB-SELECT TRAS O NUMERO QUE PESSOAS QUE FAZER PARTE DA EQUIPE DO USUARIO.
                /*
                CASE WHEN s.sercomequipe = 't'
                    THEN COALESCE( (qt.qtd_aval * 100) / (SELECT CASE WHEN COUNT(sercpf) = 0 THEN 1 ELSE COUNT(sercpf) END FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ), 0 ) ||' % Avaliaram'
                    ELSE COALESCE( (qt.qtd_aval * 100) / (SELECT CASE WHEN COUNT(sercpf) = 0 THEN 1 ELSE COUNT(sercpf) END FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = s.sercpf), 0 ) ||' % Avaliaram'
                END AS perc_membro,
                */
                CASE WHEN s.sercomequipe = 't'
                    THEN '<span style=\"font-weight: bold;\">'|| (SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercomequipe = TRUE AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ) ||' / '|| qt.qtd_aval ||'</span>'
                    ELSE '<span style=\"font-weight: bold;\">'|| (SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = s.sercpf) ||' / '|| qt.qtd_aval  ||'</span>'
                END AS membros_avaliacoes

        FROM gestaopessoa.servidor s
        JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid

        LEFT JOIN(
            SELECT  tavid, sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = 1
            GROUP BY tavid, sercpf
        ) AS a ON a.sercpf = s.sercpf

        LEFT JOIN(
            SELECT  tavid, sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = 2
            GROUP BY tavid, sercpf
        ) AS b ON b.sercpf = s.sercpf

        LEFT JOIN(
            SELECT  COUNT(resavaliacpf) AS qtd_aval,
                sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND defid = ".(int)ID_PERG_RESPAVALIACAO_QTR_RESP." AND tavid = ".TIPO_AVAL_CONSENSO."
            GROUP BY sercpf
        ) AS qt ON qt.sercpf = s.sercpf

        WHERE s.seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpfchefe}'
        ORDER BY hierarquia, s.sernome
    ";
    $cabecalho = array("Ação","SIAPE", "CPF", "Nome", "Cargo", "Equipe", "Situação", "Hierarquia", "Gratif.", "Auto-Aval.", "Aval. Superior", "Memb/Aval");
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'center', 'left', 'left', 'center', 'center', 'center', 'center', 'center' );
    $tamanho = Array('3%', '6%', '7%', '13%', '12%', '5%', '8%', '6%', '4%',  '6%', '6%', '6%', '6%');

    echo "<img src=\"../imagens/seta_retorno.gif\" style=\"margin-top:0px; margin-left:4px; \"/>";
    echo "<div style=\"margin-top:-6px; margin-left:17px; width:95%; height:320px; overflow:auto; border:1px solid #C1CDC1; \">";
    $db->monta_lista($sql, $cabecalho, 1000, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

    echo "</div>";
    die;
}

# - deletarFecheEquipe: TELA LISTAGEM DOS CHEFE/EQUIPES - DELETA O CHEFE DE UMA EQUIPE. DELETA TAMBÉM AS AVALIAÇÕES SUA E DE TODA A SUA EQUIPE.
function deletarFecheEquipe( $sercpf ){
    global $db;

    #BUSCA TODOS DA SUA EQUIPE COSNTURINDO UM "ARRAY" PARA DELETAR AS AVALIAÇÕES REALIZADAS POR ELES E A ELES.
    $sql_A = "SELECT sercpf FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}'";
    $dados_A = $db->carregarColuna($sql_A);

    #MONTA O "ARRAY" UMA STRING JÁ PRONTO PARA SQL.
    $membros = " ( ";
    foreach ($dados_A as $membro){
        $membros .= "'". $membro . "', ";
    }
    $membros = substr($membros, 1, ($p-2));
    $membros .= " )";

    $sql = "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and sercpf IN {$membros};";
    $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and resavaliacpf IN {$membros};";

    #DELETA AS AVALIAÇOES FEITAS POR ELE E A ELE.
    $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and sercpf = '{$sercpf}';";
    $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and resavaliacpf = '{$sercpf}';";

    $sql .= "
        UPDATE gestaopessoa.servidor
            SET sercpfchefe = ''
        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}' RETURNING sercpf;
    ";
    $sercpf = $db->pegaUm($sql);

    if( $sercpf > 1 ){
        $db->commit();

        return $sercpf;
    }
    die;
}

# - deletarServidorEquipe: TELA LISTAGEM DOS CHEFE/EQUIPES - DELETA O SERVIDOR DA EQUIPE. DELETA TAMBÉM AS AVALIAÇÕES AS SUAS AVALIAÇÕES.
function deletarServidorEquipe( $sercpf ){
    global $db;

    $sql = "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and sercpf = '{$sercpf}';";
    $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and resavaliacpf = '{$sercpf}';";

    $sql .= "
        UPDATE gestaopessoa.servidor
            SET sercpfchefe = ''
        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}' RETURNING sercpf;
    ";
    $sercpf = $db->pegaUm($sql);

    if( $sercpf > 1 ){
        $db->commit();

        return $sercpf;
    }
    die;
}

# - existeAvaliacaoTipo: TELA FORMULARIO DE AVALIAÇÃO - VERIFICA SE EXISTE AVALIAÇÃO FEITA AO USUARIO LOGADO SISTEMA, DE UM DETERMINADO TIPO.
function existeAvaliacaoTipo( $sercpf, $tipo = NULL ){
    global $db;

    if($tipo == NULL){
        $sercpf = $sercpf['cpf'];
        $sql = "
            SELECT MAX(resid) AS ult_resp
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}'
        ";
        $dados = $db->pegaUm($sql);
        if( $dados > 1 ){
            $resp = 'S';
        }else{
            $resp = 'N';
        }
        die($resp);
    }else{
        $sql = "
            SELECT MAX(resid) AS ult_resp
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = {$tipo} AND sercpf = '{$sercpf}'
        ";
        $dados = $db->pegaUm($sql);

        if( $dados > 1 ){
            return true;
        }else{
            return false;
        }
    }
}

# - enviarEmailPedidoReconcideracao: TELA DE PEDIDO DE RECONSIDERAÇÃO - ENVIAR E-MAIL PARA OS USUÁRIOS RELACIONADOS AO PEDIDO DE RECONCIDERAÇÃO, "CHEFE" E "USUÁRIO SOLICITANTE".
function enviarEmailPedidoReconcideracao( $fdpcpf ){
    global $db;

    $sql = "
        SELECT  s.sernome AS nome_servidor,
                s.sercpf AS cpf_servidor,
                f1.fdfemail AS email_servidor,

                CASE
                    WHEN (ftprpedido = '' OR ftprpedido = 'NA') THEN 'NÃO ANALISADO'
                    WHEN ftprpedido = 'D' THEN 'DEFERIDO'
                    WHEN ftprpedido = 'P' THEN 'DEFERIDO PARCIALMENTE'
                    WHEN ftprpedido = 'I' THEN 'INDEFERIDO'
                END ftprpedido,

                --chefe
                c.sernome AS nome_chefe,
                c.sercpf AS cpf_chefe,
                f2.fdfemail AS email_chefe
        FROM gestaopessoa.servidor AS s
        LEFT JOIN gestaopessoa.ftpedidoreconsideracao AS p ON p.fdpcpf = s.sercpf
        LEFT JOIN gestaopessoa.servidor AS c ON c.sercpf = s.sercpfchefe AND c.seranoreferencia = 2013
        LEFT JOIN gestaopessoa.ftdadofuncional AS f1 ON f1.fdpcpf = s.sercpf
        LEFT JOIN gestaopessoa.ftdadofuncional AS f2 ON f2.fdpcpf = c.sercpf
        WHERE s.seranoreferencia = {$_SESSION['exercicio']} AND s.sercpf = '$fdpcpf'
    ";
    $dados = $db->pegaLinha($sql);

    $arrEmail = array($dados['email_servidor'], $dados['email_chefe']);

    $remetente = array("nome" => "Sistema Gestaão de Pessoas - Avaliação de Servidores", "email" => $_SESSION['email_sistema']);
    $destinatario = $arrEmail;
    $assunto = "Pedido de Reconsideração - Gestão Pessoas - Avaliação de Servidores";

    $conteudo = "
        <b>Pedido de Reconsideração - Avaliação de Servidores</b>
        <p>
            E-mail infomativo,

            Esta sendo requerido ao Chefe Imediato ou ao seu Substituto, responsável pela Avaliação de Desempenho Individual, \"RECONSIDERAÇÃO\" do resultado final obtido na mesma. <br>

            Pedido de reconsideração para à Avaliação do Servidor, {$dados['nome_servidor']}.<br>

            Onde o pedido de reconsideração foi: <br>
            ( X ) {$dados['ftprpedido']}
        </p>
    ";
    $enviado = enviar_email( $remetente, $destinatario, $assunto, $conteudo );

    if( $enviado ){
        return true;
    }else{
        return false;
    }


}

# - salvarFecheEquipe: TELA CADASTRO DE CHEFE DE EQUIPE - LISTAGEM DE CHEFES/EQUIPES - SALVA O SERVIDOR SELECIONADO COMO CHEFE DE UMA "EQUIPE".
function salvarFecheEquipe( $dados ){
    global $db;

    $sercpfchefe = $dados['sercpfchefe'];
    $sercpf      = $dados['sercpf'];

    $sql_A = "SELECT sercpf FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpfchefe}'";
    $dados_A = $db->carregarColuna($sql_A);

    #MONTA O "ARRAY" UMA STRING JÁ PRONTO PARA SQL.
    $membros = " ( ";
    foreach ($dados_A as $membro){
        $membros .= "'". $membro . "', ";
    }
    $membros = substr($membros, 1, ($p-2));
    $membros .= " )";

    $sql = "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and sercpf IN {$membros};";
    $sql .= "DELETE FROM gestaopessoa.respostaavaliacao WHERE resano = {$_SESSION['exercicio']} and resavaliacpf IN {$membros};";

    $sql .= "
        UPDATE gestaopessoa.servidor
            SET sercpfchefe = '{$sercpf}'
        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpfchefe}' RETURNING sercpf;
    ";

    $dados = $db->pegaLinha($sql);

    if( $dados['sercpf'] > 1 ){
        $db->commit();
        echo '<resp>OK</resp>';
    }
    die;
}

# - salvarHistoricoAlteracaoFeche: TELA CADASTRO JUSTIFICATIVA DE ALTERAÇÃO OU EXCLUSÃO DE CHEFES - SALVA A AÇÃO DE MANUTENÇÃO EFETUADA. PODENDO SER EXCLUSÃO OU ALTERAÇÃO.
function salvarHistoricoAlteracaoFeche( $dados ){
    global $db;

    $sercpf     = $dados['sercpf'];
    $tmeid      = $dados['tmeid'];
    $hmemotivo  = $dados['hmemotivo'];
    $tipo_serv  = $dados['tipo_serv'];

    if( $dados ['tipo_acao'] == 'A' ){

        $sql = "
            INSERT INTO gestaopessoa.histmanutencaoequipe(
                    usucpf,
                    tmeid,
                    sercpf,
                    seranoreferencia,
                    hmedtinclusao,
                    hmemotivo,
                    hmeacao
                )VALUES(
                    '{$_SESSION['usucpf']}',
                    {$tmeid},
                    '{$sercpf}',
                    '{$_SESSION['exercicio']}',
                    'NOW()',
                    '$hmemotivo',
                    'A'
                )RETURNING hmeid;
        ";
        $dados = $db->pegaUm($sql);

        if($dados > 0){
            $db->commit();
            $db->sucesso( 'principal/avaliacao_servidor/cad_alteracao_feche_equipe&acao=A', '&sercpfchefe='.$sercpf, 'Já foi registrado a justificativa da alteração. De continuidade ao trabalho!' );
            die;
        }
    }else{
        $sql = "
            INSERT INTO gestaopessoa.histmanutencaoequipe(
                    usucpf,
                    tmeid,
                    sercpf,
                    seranoreferencia,
                    hmedtinclusao,
                    hmemotivo,
                    hmeacao
                )VALUES(
                    '{$_SESSION['usucpf']}',
                    {$tmeid},
                    '{$sercpf}',
                    '{$_SESSION['exercicio']}',
                    'NOW()',
                    '$hmemotivo',
                    'E'
                )RETURNING hmeid;
        ";
        $dados = $db->pegaUm($sql);

        if($dados > 0){
            $db->commit();

            if( $tipo_serv == 'CH' ){
                $deletado = deletarFecheEquipe( $sercpf );
            }else{
                $deletado = deletarServidorEquipe( $sercpf );
            }

            if( $deletado > 1 ){
                $db->sucesso( 'principal/avaliacao_servidor/lista_grid_feche_equipe&acao=A', '', 'Já foi registrado a justificativa da Exclução. A exclusão foi realizada com sucesso!', 'S', 'S' );
            }else{
                $db->sucesso( 'principal/avaliacao_servidor/lista_grid_feche_equipe&acao=A', '', 'Ocorreu um problema, tente novamente mais tarde.', 'S', 'S' );
            }
            die;
        }
    }
}

# - salvarServidorEquipe: TELA CADASTRO DE SERVIDOR A EQUIPE - LISTAGEM DE SERVIDORES - SALVA O SERVIDOR SELECIONADO A UMA "EQUIPE", VINCULA O CPF DO USUARIO SELECIONADO AO "CHEFE".
function salvarServidorEquipe( $dados ){
    global $db;

    $sercpfchefe = $dados['sercpfchefe'];
    $sercpf      = $dados['sercpf'];

    $sql = "
        UPDATE gestaopessoa.servidor
            SET sercpfchefe = '{$sercpfchefe}'
        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}' RETURNING sercpf;
    ";
    $dados = $db->pegaLinha($sql);

    if( $dados['sercpf'] > 1 ){
        $db->commit();
        echo '<resp>OK</resp>';
    }
    die;
}

# - salvarPedidoReconsideracao: TELA PEDIDO DE RECONSIDERAÇÃO - SALVA O PEDIDO DE RECONSIDERAÇÃO.
function salvarPedidoReconsideracao( $dados ){
    global $db;

    extract( $dados );

    $ftprboletimserv = $ftprboletimserv ? $ftprboletimserv : 'null';
    $ftprdtboletimserv = $ftprdtboletimserv ? "'".formata_data_sql($ftprdtboletimserv)."'" : 'null';

    if( $ftprid ){
        $sql = "
            UPDATE gestaopessoa.ftpedidoreconsideracao SET
                  ftprboletimserv                       = $ftprboletimserv,
                  ftprdtboletimserv                     = $ftprdtboletimserv,
                  ftprconmettecdesatcaref               = '$ftprconmettecdesatcaref',
                  ftprpontrecebida                      = '$ftprpontrecebida',
                  ftprpontsolicitada                    = '$ftprpontsolicitada',
                  ftprprodtrabalho                      = '$ftprprodtrabalho',
                  ftprpontrecebidaprodtrab              = '$ftprpontrecebidaprodtrab',
                  ftprpontsolicprodtrab                 = '$ftprpontsolicprodtrab',
                  ftprcapautdesen                       = '$ftprcapautdesen',
                  ftprpontrecebcapdesen                 = '$ftprpontrecebcapdesen',
                  ftprpontsoliccapdesen                 = '$ftprpontsoliccapdesen',
                  ftprrelacinterpessoal                 = '$ftprrelacinterpessoal',
                  ftprpontrecebrelintpes                = '$ftprpontrecebrelintpes',
                  ftprpontsolicrelintpes                = '$ftprpontsolicrelintpes',
                  ftprtrabequipe                        = '$ftprtrabequipe',
                  ftprpontrecebtrabequipe               = '$ftprpontrecebtrabequipe',
                  ftprpontsolictrabequipe               = '$ftprpontsolictrabequipe',
                  ftprcomprtrabalho                     = '$ftprcomprtrabalho',
                  ftprpontrecebcomptrab                 = '$ftprpontrecebcomptrab',
                  ftprpontsoliccomptrab                 = '$ftprpontsoliccomptrab',
                  ftprcumpnorproccodatrcarg             = '$ftprcumpnorproccodatrcarg',
                  ftprpontrecebcumpnorproccodatrcarg    = '$ftprpontrecebcumpnorproccodatrcarg',
                  ftprpontsoliccumpnorproccodatrcarg    = '$ftprpontsoliccumpnorproccodatrcarg',
                  flprparcerchefia                      = '$flprparcerchefia',
                  ftprpedido                            = '$ftprpedido',
                  ftprcienavaliador                     = '$ftprcienavaliador'

            WHERE ftprid = {$ftprid} RETURNING ftprid;
        ";

        $sql .= " UPDATE gestaopessoa.ftdadofuncional SET fdftelefone = '{$fdftelefone}' WHERE fdpcpf = '{$fdpcpf}' RETURNING fdfid;";
        $DADOS = $db->executar( $sql );
    } else {
        $sql = "
            INSERT INTO gestaopessoa.ftpedidoreconsideracao(
                    fdpcpf, ftprboletimserv, ftprdtboletimserv,
                    ftprconmettecdesatcaref, ftprpontrecebida, ftprpontsolicitada,
                    ftprprodtrabalho, ftprpontrecebidaprodtrab, ftprpontsolicprodtrab,
                    ftprcapautdesen, ftprpontrecebcapdesen, ftprpontsoliccapdesen,
                    ftprrelacinterpessoal, ftprpontrecebrelintpes, ftprpontsolicrelintpes,
                    ftprtrabequipe, ftprpontrecebtrabequipe, ftprpontsolictrabequipe,
                    ftprcomprtrabalho, ftprpontrecebcomptrab, ftprpontsoliccomptrab,
                    ftprcumpnorproccodatrcarg, ftprpontrecebcumpnorproccodatrcarg, ftprpontsoliccumpnorproccodatrcarg,
                    flprparcerchefia, ftprpedido, ftprcienavaliador, anoreferencia
                )VALUES(
                    '$fdpcpf', $ftprboletimserv, $ftprdtboletimserv,
                    '$ftprconmettecdesatcaref', '$ftprpontrecebida', '$ftprpontsolicitada',
                    '$ftprprodtrabalho', '$ftprpontrecebidaprodtrab', '$ftprpontsolicprodtrab',
                    '$ftprcapautdesen', '$ftprpontrecebcapdesen', '$ftprpontsoliccapdesen',
                    '$ftprrelacinterpessoal', '$ftprpontrecebrelintpes', '$ftprpontsolicrelintpes',
                    '$ftprtrabequipe', '$ftprpontrecebtrabequipe', '$ftprpontsolictrabequipe',
                    '$ftprcomprtrabalho', '$ftprpontrecebcomptrab', '$ftprpontsoliccomptrab',
                    '$ftprcumpnorproccodatrcarg', '$ftprpontrecebcumpnorproccodatrcarg', '$ftprpontsoliccumpnorproccodatrcarg',
                    '$flprparcerchefia', '$ftprpedido', '$ftprcienavaliador', '{$_SESSION['exercicio']}'
                )RETURNING ftprid;
        ";

        $sql .= " UPDATE gestaopessoa.ftdadofuncional SET fdftelefone = '{$fdftelefone}' WHERE fdpcpf = '{$fdpcpf}';";
        $DADOS = $db->executar( $sql );
    }

    if( $DADOS > 0  ){
        $db->commit();

        enviarEmailPedidoReconcideracao( $fdpcpf );

        $db->sucesso( 'principal/avaliacao_servidor/cad_pedido_reconsideracao' );
    } else {
        $db->insucesso( 'Operação não realizada, por favor tente novamente mais tarde!', '', $modulo='principal/avaliacao_servidor/cad_pedido_reconsideracao' );
    }
    exit();
}

# - verificaDadosServidorAtualizados: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - VERIFICA SE OS DADOS PESSOAIS E FUNCIONAIS ESTÃO DEVIDAMENTE PREENCHIDOS.
function verificaDadosServidorAtualizados( $dados ){
    global $db;

    header("Content-Type: text/html; charset=ISO-8859-1");

    $fdpcpf     = $dados['sercpf'];

    $controle   = 'OK';
    $msg        = "\nÉ necessário atualizar os dados no Módulo FORÇA DE TRABALHO, a(s) aba(s) é/são: \n";

    $resp = verificaTipoGratificacaoServidor( $fdpcpf );

    if( $resp['tssid'] != 8 && $resp['sertipogratificacao'] != 'PS' ){
        $sql = "
            SELECT  p.fdpcpf,
                    f.fdfid,
                    a.ffaid,
                    i.fidif,
                    d.fadid,
                    x.feaid,
                    c.ffcid
            FROM gestaopessoa.ftdadopessoal AS p

            LEFT JOIN gestaopessoa.ftdadofuncional AS f ON f.fdpcpf = p.fdpcpf AND fdfatualizacao = {$_SESSION['exercicio']}

            LEFT JOIN(
                SELECT COUNT(ffaid) AS ffaid, fdpcpf
                FROM gestaopessoa.ftformacaoacademica
                GROUP BY fdpcpf
            ) AS a ON a.fdpcpf = p.fdpcpf

            LEFT JOIN(
                SELECT COUNT(fidif) AS fidif, fdpcpf
                FROM gestaopessoa.idioma
                GROUP BY fdpcpf
            ) AS i ON i.fdpcpf = p.fdpcpf

            LEFT JOIN(
                SELECT COUNT(fadid) AS fadid, fdpcpf
                FROM gestaopessoa.ftatividadedesenvolvida
                GROUP BY fdpcpf
            ) AS d ON d.fdpcpf = p.fdpcpf

            LEFT JOIN(
                SELECT COUNT(feaid) AS feaid, fdpcpf
                FROM gestaopessoa.ftexperienciaanterior
                GROUP BY fdpcpf
            ) AS x ON x.fdpcpf = p.fdpcpf

            LEFT JOIN(
                SELECT COUNT(ffcid) AS ffcid, fdpcpf
                FROM gestaopessoa.ftformacaocurso
                GROUP BY fdpcpf
            ) AS c ON c.fdpcpf = p.fdpcpf

            WHERE p.fdpcpf = '{$fdpcpf}' AND fdpatualizacao = {$_SESSION['exercicio']}
        ";
        $dados = $db->pegaLinha($sql);

        if( $dados['fdpcpf'] == '' ){
            $msg .= "- DADOS PESSOAIS. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['fdfid'] == '' ){
            $msg .= "- DADOS FUNCIONAIS. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['ffaid'] == '' || $dados['ffaid'] == 0 ){
            $msg .= "- FORMAÇÃO ACADÊMICA. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['fidif'] == '' || $dados['ffaid'] == 0 ){
            $msg .= "- IDIOMAS. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['fadid'] == '' || $dados['fadid'] == 0 ){
            $msg .= "- ATIVIDADES DESENVOLVIDAS. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['feaid'] == '' || $dados['feaid'] == 0 ){
            $msg .= "- EXPERIÊNCIAS ANTERIORES. \n";
            $controle = 'ATUALIZAR';
        }
        if( $dados['ffcid'] == '' || $dados['ffcid'] == 0 ){
            $msg .= "- CURSOS. \n";
            $controle = 'ATUALIZAR';
        }
    }

    if( $controle == 'OK' ){
        die("<resp>OK</resp>");
    }else{
        $_SESSION['fdpcpf'] = $fdpcpf;

        $msg .= "\nVocê será direcionado ao Módulo \"FORÇA DE TRABALHO\" para atualizar seus dados. Ao concluir a atualização, clique: \n";
        $msg .= "Menu Principal -> Página Principal. \nE dê continuidade ao trabalho de Avaliação.";
        die("<resp>{$msg}</resp>");
    }
}

# - verificaTipoGratificacaoServidor: TELA CADASTRO DA AVALIAÇÃO - AMD E AVALIAÇÃO - VERIFICA QUAL É TIPO DE GRATIFICAÇÃO TEM O SERVIDOR.
function verificaTipoGratificacaoServidor( $fdpcpf ){
    global $db;

    #(PE)-CGPGPE; (CE)-GDACE; (PS)-GDAPS; (OS)-CEDIDOS; (FE)-CHEFES
    $sql = "
        SELECT  sertipogratificacao, tssid

        FROM gestaopessoa.servidor

        WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$fdpcpf}'
    ";
    return $db->pegaLinha($sql);
}

# - verificarTipoAvaliacao: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - VERIFICA QUAL É TIPO DE AVALIAÇÃO "AUTO-AVALIAÇÃO" OU NÃO. E SETA AS VARIAVEIS DE SESSÃO.
function verificarTipoAvaliacao( $dado ){
    global $db;

    unset($_SESSION['cpfavaliado']);

    $sercpf = $dado['sercpf'];

    $sql = "
        SELECT * FROM gestaopessoa.servidor WHERE sercpf = '{$sercpf}' AND seranoreferencia = {$_SESSION['exercicio']} AND serstatus = TRUE;
    ";
    $dados = $db->pegaLinha($sql);

    if( $dados['sercpfchefe'] == $_SESSION['usucpf'] ){#AVALIAÇÃO CHEFIA
        $_SESSION['boautoavaliacao'] = false;
        $_SESSION['cpfavaliado'] = $dados['sercpf'];
        $_SESSION['autoavalchefe'] = true;
    }elseif( $dados['sercpf'] == $_SESSION['usucpf'] ){#AUTO AVALIAÇÃO
        $_SESSION['boautoavaliacao'] = true;
        $_SESSION['cpfavaliado'] = $dados['sercpf'];
        $_SESSION['autoavalchefe'] = false;
    }else{#AVALIAÇÃO EQUIPE
        $_SESSION['boautoavaliacao'] = false;
        $_SESSION['cpfavaliado'] = $dados['sercpf'];
        $_SESSION['autoavalchefe'] = false;
    }
    die('<resp>OK</resp>');
}

# - tratamentoMembrosEquipe: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - MONTA A EQUIPE SO RESPECTIVO SERVIDOR SENDO FECHE OU SUBORDINADO.
function tratamentoMembrosEquipe( $sercpf ){
    global $db;

    $RESP = verificaTipoGratificacaoServidor( $sercpf );

    if( $RESP['tssid'] != 8 && $RESP['sertipogratificacao'] != 'FE' && $RESP['sertipogratificacao'] != 'PS' ){
        #BUSCA DO USUÁRIO SE O MESMO TEM OU FAZ PARTE DE EQUIPE. NO CAMPO "sercomequipe", CASO TRUE (TEM EQUIPE) CASO FALSE (NÃO TEM EQUIPE).
        $sql = "
            SELECT  CASE WHEN sercomequipe = TRUE
                        THEN 'S'
                        ELSE 'N'
                    END AS equipe
            FROM gestaopessoa.servidor
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}'
        ";
        $dados_eqp = $db->pegaUm($sql);

        #BUSCA DO USUÁRIO PELO CPF NO CAMPO "sercpfchefe", CASO O CPF SE REPITA POR MAIS DE UMA VEZ, O USUÁRIO É "CHEFE" DE UMA EQUIPE.
        $sql = "
            SELECT count(sercpf)
            FROM gestaopessoa.servidor
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}'
        ";
        $dados_qtd = $db->pegaUm($sql);

        #BUSCA O CPF NO CAMPO "sercpfchefe" DO RESPECTIVO USUÁRIO, BUSCANCO O O SEU "CHEFE".
        $sql = "
            SELECT sercpfchefe, sernivelfuncao AS funcao
            FROM gestaopessoa.servidor
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}'
        ";
        $dados_chefe = $db->pegaLinha($sql);

        #DA INICIO A MONTAGEM DA EQUIPE, PRIMEIRO VERIFICA SE O USUARIO PARTICIPA DE UMA EQUIPE. SE SIM É DIRECIONADO A UMA REGRA DE MONTAGEM DE EQUIPE ESPECIFICA, CASO NÃO É DIRECIONADO A OUTRA REGRA.
        if( $dados_eqp == 'S' ){

            #SE "$dados_qtd" FOR IGUAI A 0, SIGNIFICA QUE O USUÁRIO NÃO É FECHE, ENTÃO É BUSCADO O SEU RESPCTIVO FECHE.
            if($dados_qtd == 0){
                #BUSCADO O RESPCTIVO FECHE DO USUARIO.
                $sql = "
                    SELECT sercpfchefe FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}';
                ";
                $sercpfchefe = $db->pegaUm($sql);

                #BUSCA OS SERVIDORES REALACIONADOS COM O "sercpfchefe" ENCONTRADO. NA SQL ANTERIOR.
                $sql = "
                    SELECT  sercpf
                    FROM gestaopessoa.servidor
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpfchefe}' AND sercomequipe = TRUE
                ";
                $equipe = $db->carregarColuna($sql);

                #ADICIONA O "sercpfchefe" "O CHEFE" NA VARIAVEL PARA QUE O MESMO SEJA PARTE DA EQUIPE, SEJA INCLUIDO NA LISTAGEM DA EQUIPE.
                $addMembro = $sercpfchefe;

            }else{
                $sql = "
                    SELECT  sercpf
                    FROM gestaopessoa.servidor
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND (sercpfchefe = '{$dados_chefe['sercpfchefe']}' OR sercpfchefe = '{$sercpf}')
                ";
                $equipe = $db->carregarColuna($sql);

                #ADICIONA O "sercpfchefe" "O CHEFE" NA VARIAVEL PARA QUE O MESMO SEJA PARTE DA EQUIPE, SEJA INCLUIDO NA LISTAGEM DA EQUIPE.
                $addMembro = $dados_chefe['sercpfchefe'];
            }
        }else{
            #BUSCA OS SERVIDORES REALACIONADOS COM O "sercpfchefe" DO MESMO.
            $sql = "
                SELECT sercpf FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}'
            ";
            $equipe = $db->carregarColuna($sql);

            #ADICIONA AO ARRAY DE EQUIPE O PROPIO. ISSO SE FAZ NECESSARIO PARA QUE O MESMO ESTEJA NA LISTAGEM PARA ALTO-AVALIAÇÃO.
            array_push($equipe, $sercpf);

            #ADICIONA O "sercpfchefe" "O CHEFE" NA VARIAVEL PARA QUE O MESMO SEJA PARTE DA EQUIPE, SEJA INCLUIDO NA LISTAGEM DA EQUIPE.
            $addMembro = $dados_chefe['sercpfchefe'];
        }
    }

    if( $RESP['sertipogratificacao'] == 'FE' || $RESP['sertipogratificacao'] == 'PS' || $RESP['tssid'] == 8){
        #BUSCA DO USUÁRIO PELO CPF NO CAMPO "sercpfchefe", CASO O CPF SE REPITA POR MAIS DE UMA VEZ, O USUÁRIO É "CHEFE" DE UMA EQUIPE.
        $sql = "
            SELECT count(sercpf)
            FROM gestaopessoa.servidor
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}'
        ";
        $chefia = $db->pegaUm($sql);

        if( $RESP['sertipogratificacao'] == 'FE' ){
            #BUSCA O CPF NO CAMPO "sercpf" DO RESPECTIVO USUÁRIO, BUSCANCO O SEU SUBORDINADO.
            $sql = "
                SELECT  sercpf
                FROM gestaopessoa.servidor
                WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}' AND tssid = 8
            ";
            $equipe = $db->carregarColuna($sql);
            
            #BUSCA CHEFE DO USUÁRIO/AVALIADOR EXTERNO - CASO TENHA CHEFE ELE SERÁ AVALIADO.
            $sql = "
                SELECT count(sercpfchefe)
                FROM gestaopessoa.servidor
                WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf = '{$sercpf}'
            ";
            $tem_chefe = $db->pegaUm($sql);

            if( $tem_chefe > 0){
                #ADICIONA AO ARRAY EQUIPE O PROPRIO. PARA QUE ELE ACOMPANHE A SUA AVALIAÇÃO.
                array_push($equipe, $sercpf);
            }
        }

        if( $RESP['sertipogratificacao'] == 'PS' && $chefia > 0){
            #BUSCA O CPF NO CAMPO "sercpf" DO RESPECTIVO USUÁRIO, BUSCANCO O SEU SUBORDINADO.
            $sql = "
                SELECT  sercpf
                FROM gestaopessoa.servidor
                WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = '{$sercpf}'
            ";
            $equipe = $db->carregarColuna($sql);

            #ADICIONA AO ARRAY DE EQUIPE O PROPIO. ISSO SE FAZ NECESSARIO PARA QUE O MESMO ESTEJA NA LISTAGEM PARA ALTO-AVALIAÇÃO.
            array_push($equipe, $sercpf);
        }

        #QUANDO A USUÁRIO TEM A GRATIFICAÇÃO CEDIDO, ELE NÃO FAZ AUTO AVALIAÇÃO, AVALIAÇÃO DE EQUIPE E NÃO AVALIADO PELA EQUIPE A APENAS AVALIAÇÃO DA CHEFIA. ASSIM SENDO ELE VISUALIZA APENAS A SI MESMO NA LISTAGEM.
        if( ($RESP['tssid'] == 8 || $RESP['sertipogratificacao'] == 'PS') && $chefia == 0 ){
            $equipe = array($sercpf);
        }
    }

    #MONTA O "ARRAY" UMA STRING JÁ PRONTO PARA SQL.
    $membros = " ( ";

    foreach ($equipe as $membro){
        $membros .= "'". $membro . "', ";
    }

    $membros .= "'".$addMembro."'";
    $membros .= " )";

    return $membros;
}

# - verificaSeHaResposta: USADO NA TELA DE CADASTRO DE AVALIAÇÃO - VERIFICA SE A AVALIAÇÃO FEITA AO USUÁRIO SELECIONADO. CASO TENHA É POSSIVEL QE O USUÁRIO COM O PERFIL SUPER OU ADM, VEJA A VALIAÇÃO COMPLETA.
function verificaSeHaResposta(  ){
    global $db;

    $sql = "
        SELECT  resid
        FROM gestaopessoa.respostaavaliacao
        WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND resano = {$_SESSION['exercicio']} AND tavid IN (1, 2, 3)
    ";
    $resid = $db->pegaUm( $sql );

    return $resid;
}

# - quantidadeServidorAvaliados: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - CONTA A QUANTIDADE SERVIDORES AVALIADOS.
function quantidadeServidorAvaliados(){
    global $db;

	$sql = "
        SELECT  COUNT( DISTINCT(sercpf) ) AS qtd_avaliacoes
        FROM gestaopessoa.respostaavaliacao
        WHERE resano = {$_SESSION['exercicio']} AND resavaliacaopendente = 't' AND defid = ".(int)ID_PERG_RESPAVALIACAO_QTR_RESP."
    ";
    $dados = $db->pegaUm( $sql );

    if($dados){
        return $dados;
    }else{
        return 0;
    }
}

# - quantidadeServidorAvaliacaoFinalizada: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - CONTA A QUANTIDADE SERVIDORES COM A AVALIAÇÃO FINALIZADA.
function quantidadeServidorAvaliacaoFinalizada(){
    global $db;

	$sql = "
        SELECT  COUNT( DISTINCT(sercpf) ) AS qtd_avaliacoes
        FROM gestaopessoa.respostaavaliacao
        WHERE resano = {$_SESSION['exercicio']} AND resavaliacaopendente = 'f' AND defid = ".(int)ID_PERG_RESPAVALIACAO_QTR_RESP."
    ";
	$dados = $db->pegaUm( $sql );

    if($dados){
        return $dados;
    }else{
        return 0;
    }
}

# - quantidadeServidorAvaliados: TELA LISTAGEM DE SERVIDORES - AMD E AVALIAÇÃO - CONTA A QUANTIDADE SERVIDORES CADASTRADOS.
function quantidadeServidorCadastrados(){
    global $db;

	$sql = "
        SELECT  COUNT(s.sercpf) qtd_servidores
        FROM gestaopessoa.servidor AS s
        WHERE s.seranoreferencia = {$_SESSION['exercicio']}
    ";
	$dados = $db->pegaUm( $sql );

	if($dados){
        return $dados;
    }else{
        return 0;
    }
}


#----------------------------------------------------------- FUNÇÕES MODULO CESSÃO/PRORROGAÇÃO SERVIDOR ----------------------------------------------#
#AS FUNÇÕES SÃO: (EM ORDER ALFABETICA)
# - atualizaComboUndCad;
# - atualizaComboAdvogados;
# - anexarDocumentos;
# - anexarPortaria;
# - atualizaGridCessao;
# - atualizaGridProrrogacao;
# - buscarDadosServidor;
# - buscarDadosPortaria;
# - cabecalhoAbas;
# - dadosServidor;
# - deletaProcessoCessaoProrrogacao;
# - editarPortaria;
# - excluirDocAnexo;
# - monta_abas_cessao_prorrogacao;
# - salvarDadosCessao;
# - salvarParecer;
# - salvarParecerUni;
# - salvarPortaria;
# - salvarDadosProrrogacao;


# - atualizaComboUndCad: TELA CADASTRO DE CESSÃO/PRORROGAÇÃO - ATUALIZA O CAMBO ORIGEM FLUXO DE ACORDO COM AS REGRAS.
function atualizaComboUndCad( $dados ){
    global $db;

    $tpsid = $dados['tpsid'];

    switch ( $tpsid ){
        case 1:
            $where = "WHERE orcid IN (1)";
            break;
        case 2:
            $where = "WHERE orcid IN (1, 3)";
            break;
        case 3:
            $where = "WHERE orcid IN (2)";
            break;
        default:
            $where = "WHERE orcid IN (1, 2, 3, 4, 5, 6, 7)";
    }

    $sql = "
        SELECT  orcid AS codigo,
                orcdsc AS descricao
        FROM gestaopessoa.origemcessao
        {$where}
        ORDER BY orcdsc
    ";
    $db->monta_combo("orcid", $sql, 'S', 'Selecione...', '', '', '', '336', 'S', 'orcid', false, $orcid, null);
    die();
}

# - atualizaComboAdvogados: TELA CADASTRO DE PARECER DE CESSÃO - ATUALIZA O CAMBO ADVOGADOS. FLUXO DE ACORDO COM AS REGRAS.
function atualizaComboAdvogados( $dados ){
    global $db;

    header("Content-Type: text/html; charset=ISO-8859-1");
    if($dados['coonid'] > 0){
        $sql = "
            SELECT 	distinct
                    --c.coonid,
                    --c.coodsc,
                    a.advid  AS codigo,
                    --e.entid,
                    e.entnome AS descricao
            FROM conjur.coordenacao c
            JOIN conjur.advogadosxcoordenacao ac ON ac.coonid = c.coonid
            JOIN conjur.advogados a ON a.advid = c.advid
            JOIN entidade.entidade e ON e.entid = a.entid
            WHERE a.advstatus = 'A' AND c.coonid = {$dados['coonid']}
        ";
        $db->monta_combo("advid", $sql, 'S', 'Selecione...', '', '', '', '370', 'S', 'advid', false, $advid, null);
        die();
    }else{
        die();
    }
}

# - anexarDocumentos: TELA CADASTRO DE CESSÃO - ANEXAR DOCUMENTOS.
function anexarDocumentos($dados, $files) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $cprid = $dados['cprid'];

    $aqcdtinclusao = "'".gmdate('Y-m-d')."'";

    $campos = array(
        "cprid"         => $cprid,
        "aqcdsc"        => "'".addslashes( $dados['aqcdsc'] )."'",
        "tpdid"         => $dados['aqctipodoc'],
        "aqcstatus"     => "'A'",
        "aqcdtinclusao" => $aqcdtinclusao
    );

    $file = new FilesSimec("arqcessao", $campos, "gestaopessoa");

    if ( $files ) {
        $arquivoSalvo = $file->setUpload("Gestão Pessoas-Cadastro de Documentos", "arquivo");
        if ($arquivoSalvo) {
            $_SESSION['gestao']['cprid'] = $cprid;

            if($dados['continuar'] == 'S'){
                $db->sucesso('principal/cessao_prorrogacao/cad_dados_portaria', '&acao=A');
            }else{
                $db->sucesso('principal/cessao_prorrogacao/cad_dados_documentos', '&acao=A');
            }
            exit();
        }
    }
    exit;
}

# - anexarPortaria: TELA CADASTRO DA PORTARIA - CESSÃO - ANEXAR PORTARIA.
function anexarPortaria($dados, $files) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $cprid          = $dados['cprid'];
    $dpcnumero      = trim($dados['dpcnumero']);
    $dpcdsc         = trim($dados['dpcdsc']);
    $dpcdtportaria  = formataDataBanco( $dados['dpcdtportaria'] );
    $dpcdtinclusao  = gmdate('Y-m-d');

    $campos = array(
        "cprid"         => $cprid,
        "dpcnumero"     => "'".$dpcnumero."'",
        "dpcstatus"     => "'A'",
        "dpcdtinclusao" => "'".$dpcdtinclusao."'",
        "dpcdsc"        => "'".addslashes( $dpcdsc )."'",
        "dpcdtportaria" => "'".$dpcdtportaria."'"
    );

    $file = new FilesSimec("docportaria", $campos, "gestaopessoa");

    if ( $files ) {
        $arquivoSalvo = $file->setUpload("Gestão Pessoas-Cadastro de Portaria", "arquivo");
        if ($arquivoSalvo) {
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_portaria', '&acao=A');
            exit();
        }
    }
    exit;
}

# - atualizaGridCessao: TELA LISTA DE CESSÃO E PRORROGAÇÃO CADASTRO DE PRORROGRAÇÃO - CRIA A LISTA DAS CESSOES RELACIONADOS COM O SERVIDOR(GRID).
function atualizaGridCessao( $dados ){
    global $db;

    header("Content-Type: text/html; charset=ISO-8859-1");

    $tipo = trim($dados['tipo']);
    $orcid = trim($dados['orcid']);
    $nu_matricula_siape = trim($dados['nu_matricula_siape']);

    if($orcid != ''){
        $acao = "
            CASE WHEN cp.orcid = {$orcid}
                THEN '<input type=\"radio\" id=\"selec_orcid\" name=\"selec_orcid\" onclick=\"atualizaGridProrrogacao('||c.orcid||', '||c.tpsid||');\" checked=\"checked\">'
                ELSE '<input type=\"radio\" id=\"selec_orcid\" name=\"selec_orcid\" onclick=\"atualizaGridProrrogacao('||c.orcid||', '||c.tpsid||');\">'
            END
        ";
    }else{
        $acao = "
            '<input type=\"radio\" id=\"selec_orcid\" name=\"selec_orcid[]\" onclick=\"atualizaGridProrrogacao('||c.orcid||', '||c.tpsid||');\">'
        ";
    }

    $sql = "
        SELECT  {$acao} as acao,
                c.cprnumprocesso,
                rc.orcdsc,
                ts.tpsdsc,
                oc.ogcdsc,
                c.cprcodigosimbolo,
                to_char(cprperiodo, 'MM/DD/YYYY') as cprperiodo
        FROM gestaopessoa.cessaoprorrogacao c

        JOIN siape.tb_servidor_simec AS s ON cast(s.nu_matricula_siape as character(7)) = c.nu_matricula_siape

        JOIN gestaopessoa.orgaocessionario AS oc ON oc.ogcid = c.ogcid
        JOIN gestaopessoa.origemcessao AS rc ON rc.orcid = c.orcid
        JOIN gestaopessoa.tiposolicitacao AS ts ON ts.tpsid = c.tpsid

        WHERE c.cprstatus = 'A' AND cprtipo = 'C' AND c.nu_matricula_siape =  '{$nu_matricula_siape}'
        ORDER BY cprid

    ";
    $cabecalho = Array("", "Nº do Processo", "Unidade de Cadastro", "Tipo de Solicitação", "ORG. Cessionário", "Código/Simbolo", "Prazo");
    $whidth = Array('5%','10%', '15%', '25%', '20%', '15%');
    $align  = Array('center', 'left', 'left', 'left', 'right');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

    if($tipo == 'at'){
        die();
    }
}

# - atualizaGridProrrogracao: TELA LISTA DE CESSÃO E PRORROGAÇÃO CADASTRO DE PRORROGRAÇÃO - CRIA A LISTA DAS PRORROGAÇÕES RELACIONADOS COM AS CESSÕES DO SERVIDOR(GRID).
function atualizaGridProrrogacao( $dados ){
    global $db;

    header("Content-Type: text/html; charset=ISO-8859-1");

    $tipo = trim($dados['tipo']);
    $orcid = trim($dados['orcid']);
    $nu_matricula_siape = trim($dados['nu_matricula_siape']);

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarProcessoProrrogacao('||c.cprid||')\" title=\"Editar Servidor\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaDadosProrrogacao('||c.cprid||')\" title=\"Editar Servidor\" >
    ";

    $sql = "
        SELECT  '{$acao}' as acao,
                c.cprnumprocesso,
                o.ogcdsc,
                c.cprcodigosimbolo,
                to_char(c.cprperiodo, 'MM/DD/YYYY') as cprperiodo,
                CASE WHEN c.cprsitmudanca = 'f'
                    THEN 'Não'
                    ELSE 'Sim'
                END AS cprsitmudanca,
                CASE WHEN c.cprsitperdaprazo = 'f'
                    THEN 'Não'
                    ELSE 'Sim'
                END AS cprsitperdaprazo,
                to_char(c.cprdtvencprorrogacao, 'DD/MM/YYY') as cprdtvencprorrogacao
        FROM gestaopessoa.cessaoprorrogacao c
        LEFT JOIN gestaopessoa.orgaocessionario AS o ON o.ogcid = c.ogcid
        WHERE c.cprstatus = 'A' AND c.cprtipo = 'P' AND c.orcid = {$orcid} AND c.nu_matricula_siape = '{$nu_matricula_siape}'
        ORDER BY 2
    ";
    $cabecalho = Array("", "Nº do Processo", "ORG. Cessionário", "Código/Simbolo", "Prazo", "Mud. Prazo", "Perda Prazo", "Data de Venc. Prorrogação", "Situação");
    //$whidth = Array('20%', '60%', '20%');
    //$align  = Array('left', 'left', 'center');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

    if($tipo == 'at'){
        die();
    }
}

# - atualizarPortaria: ATUALIZA OS DADOS DA PORTARIA - CESSÃO - TELA DE CADASTRO DA PORTARIA.
function atualizarPortaria($dados){
    global $db;

    $dpcid          = $dados['dpcid'];
    $cprid          = $dados['cprid'];
    $dpcnumero      = trim($dados['dpcnumero']);
    $dpcdsc         = trim($dados['dpcdsc']);
    $dpcdtportaria  = formataDataBanco( $dados['dpcdtportaria'] );

    unset($_SESSION['gestao']['cprid']);

    if($dpcid != ''){
        $sql = "
            UPDATE gestaopessoa.docportaria
                        SET dpcnumero       = '{$dpcnumero}',
                            dpcdsc          = '{$dpcdsc}',
                            dpcdtportaria   = '{$dpcdtportaria}'
                WHERE dpcid = {$dpcid} RETURNING dpcid;
        ";
        $dado = $db->pegaLinha($sql);
    }

    if( $dado > 0 ){
        $db->commit();
        $_SESSION['gestao']['cprid'] = $cprid;
        $db->sucesso('principal/cessao_prorrogacao/cad_dados_portaria', '');
    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_processo');
    }

}

# - buscarDadosServidor: BUSCA DO DADOS DO SERVIDOR -  DADOS ESSES PARA O PREENCHIMENTO DOS CAMPOS (Código SIAPE, Nome do Servidor, Situação do Servidor, Cargo Efetivo, Orgão de Origem) NA TELA CADASTRO DA CESSÃO E PRORROGAÇÃO.
function buscarDadosServidor($dados){
    global $db;

    $nu_matricula_siape = trim($dados['nu_matricula_siape']);
    $nu_cpf             = trim( str_replace( ".", "", str_replace( "-", "", $dados['nu_cpf'] ) ) );

    $tipo = trim($dados['tipo']); # O TIPO DEFINI QUAL O PROCESSO "TELA" ESTA REQUISITANDO A INFORMAÇÃO.

    if( $nu_matricula_siape != '' ){
        $where = "WHERE s.nu_matricula_siape = '{$nu_matricula_siape}'";
    }else{
        $where = "WHERE s.nu_cpf = '{$nu_cpf}'";
    }

    $sql = "
        SELECT	s.nu_matricula_siape,
                trim( replace( to_char( cast(s.nu_cpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS nu_cpf,
                trim( s.no_servidor ) AS no_servidor,
                s.co_orgao,
                s.co_situacao_servidor,
                s.co_funcao,
                s.co_cargo_emprego
        FROM siape.tb_servidor_simec s

        {$where}

        ORDER BY 1
    ";
    $dados = $db->pegaLinha($sql);

    if($dados != ''){
        if($tipo != 'C' || $tipo == ''){
            $dados["no_servidor"] = iconv("ISO-8859-1", "UTF-8", $dados["no_servidor"]);
            echo simec_json_encode($dados);
            die;
        }else{
            return $dados;
        }
    }else{
        $dados["nu_matricula_siape"] = '0';
        echo simec_json_encode($dados);
        die;
    }


}

# - buscarDadosPortaria: BUSCA DADOS DO CADASTRO DA PORTARIA PARA A EDÇÃO - USADO PELAS: TELAS CADASTRO DA PORTARIA - CESSÃO E PRORROGAÇÃO".
function buscarDadosPortaria( $dados ){
    global $db;

    $cprid = $_SESSION['gestao']['cprid'];

    $sql = "
        SELECT  dpcid,
                cprid,
                arqid,
                dpcnumero,
                dpcdsc,
                to_char(dpcdtportaria, 'DD/MM/YYYY') as dpcdtportaria
        FROM gestaopessoa.docportaria

        WHERE cprid = {$cprid}
    ";
    $dados = $db->pegaLinha($sql);
    return $dados;
}

# - buscarDadosPortaria: BUSCA DADOS DO CADASTRO DO PARECER - USADO PELAS: TELAS CADASTRO DO PARECER - CESSÃO E PRORROGAÇÃO".
function buscarDadosParecer( $dados ){
    global $db;

    $cprid = $_SESSION['gestao']['cprid'];

    $sql = "
        SELECT 	cprsitmovimento,
                cprsitcap,
                cprsitcggp,
                cprsitadvogado,
                cprsitcoordenacao,
                usucpfsitmovimentacao,
                '<span style=\"color: #0066cc\">'||u_mov.usunome||'</span>' as nome_sitmovimentacao,
                usucpfsitcap,
                '<span style=\"color: #0066cc\">'||u_cap.usunome||'</span>' as nome_sitcap,
                usucpfsitcggp,
                '<span style=\"color: #0066cc\">'||u_cggp.usunome||'</span>' as nome_sitcggp,
                usucpfsitadvogado,
                '<span style=\"color: #0066cc\">'||u_adv.usunome||'</span>' as nome_sitadvogado,
                usucpfsitcoordenador,
                '<span style=\"color: #0066cc\">'||u_coo.usunome||'</span>' as nome_sitcoordenador,
                advid,
                coonid
        FROM gestaopessoa.cessaoprorrogacao c
        LEFT JOIN (
            SELECT usucpf, usunome FROM seguranca.usuario
        ) AS u_mov ON u_mov.usucpf = c.usucpfsitmovimentacao
        LEFT JOIN (
            SELECT usucpf, usunome FROM seguranca.usuario
        ) AS u_cap ON u_cap.usucpf = c.usucpfsitcap
        LEFT JOIN (
            SELECT usucpf, usunome FROM seguranca.usuario
        ) AS u_cggp ON u_cggp.usucpf = c.usucpfsitcggp
        LEFT JOIN (
        SELECT usucpf, usunome FROM seguranca.usuario
        ) AS u_adv ON u_adv.usucpf = c.usucpfsitadvogado
        LEFT JOIN (
        SELECT usucpf, usunome FROM seguranca.usuario
        ) AS u_coo ON u_coo.usucpf = c.usucpfsitcoordenador

        WHERE cprid = {$cprid}
    ";
    $dados = $db->pegaLinha($sql);
    return $dados;
}

# - cabecalhoAbas: MONTA O CABEÇALHO DAS ABAS NO CADASTRO DE CESSÃO. USADO PELAS: TELAS CADASTRO DE CESSÃO E PRORROGAÇÃO EM TODAS AS ABAS".
function cabecalhoAbas($cprid){
    global $db;

     $sql = "
        SELECT  trim( replace(to_char(cast(s.nu_cpf as bigint), '000:000:000-00'), ':', '.') ) as nu_cpf,
                s.no_servidor,
                s.nu_matricula_siape,
                c.cprnumprocesso
        FROM siape.tb_servidor_simec AS s

        JOIN gestaopessoa.cessaoprorrogacao AS c ON c.nu_matricula_siape = cast(s.nu_matricula_siape as character(7))

        WHERE c.cprid = '{$cprid}'
        ORDER BY 1
    ";
    $dados = $db->pegaLinha($sql);

?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td class="SubTituloDireita" width="38%">CPF:</td>
        <td>
            <?php
                echo campo_texto('nu_cpf', 'N', 'N', '', 43, 10, '', '', '', '', 0, 'id="nu_cpf"', '', $dados['nu_cpf'], null, null);
            ?>
        </td>
    </tr>
    <tr>
        <td class ="SubTituloDireita">SIAPE:</td>
        <td>
            <?php
                echo campo_texto('nu_matricula_siape', 'N', 'N', '', 43, 10, '', '', '', '', 0, 'id="nu_matricula_siape"', '', $dados['nu_matricula_siape'], null, null);
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Servidor:</td>
        <td>
            <?php
                echo campo_texto('no_servidor', 'N', 'N', '', 43, 10, '', '', '', '', 0, 'id="no_servidor"', '', $dados['no_servidor'], null, null);
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nº do Porcesso:</td>
        <td>
            <?php
                echo campo_texto('cprnumprocesso', 'N', 'N', '', 43, 10, '', '', '', '', 0, 'id="cprnumprocesso"', '', $dados['cprnumprocesso'], null, null);
            ?>
        </td>
    </tr>
</table>
<br>

<?php
}

# - dadosServidor: BUSCA DADOS DO CADASTRO DE PROCESSO PARA A EDÇÃO - USADO PELAS: TELAS CADASTRO DE CESSÃO E PRORROGAÇÃO".
function dadosServidor( $dados ){
    global $db;

    unset($_SESSION['gestao']['cprid']);

    $cprid = $_GET['cprid'];

    $_SESSION['gestao']['cprid'] = $cprid;

    $sql = "
        SELECT  --PK
                cp.cprid,
                --FK's
                orcid,
                tonid,
                aimid,

                --TABELA: cessaoprorrogacao
                trim(cp.cprnumprocesso) as cprnumprocesso,
                cp.tpsid,
                cp.ogcid,
                cp.cprfuncaodesempenhada,
                cp.cprcodigosimbolo,
                cp.cprsitestagio,
                cp.cprsindpad,
                cp.cprsitdedicacao,
                cp.cprsitmudanca,
                cp.cprsitperdaprazo,
                cp.columncprprazoind,
                cp.columncprprazoano,
                to_char(cp.cprperiodo, 'DD/MM/YYYY') as cprperiodo,
                to_char(cp.cprdtvencprorrogacao, 'DD/MM/YYYY') as cprdtvencprorrogacao,
                to_char(cp.cprdtinlusao, 'DD/MM/YYYY') as cprdtinlusao,
                to_char(cp.columncprprazodata, 'DD/MM/YYYY') as columncprprazodata,

                --TABELA: orgaocessionario
                o.ogcsigla||' - '||o.ogcdsc as ogcdsc,

                --TABELA: tb_servidor_simec
                trim(replace(to_char(cast(s.nu_cpf as bigint), '000:000:000-00'), ':', '.')) as nu_cpf,
                s.nu_matricula_siape,
                s.no_servidor as no_servidor,
                s.co_funcao,
                s.co_orgao,
                s.co_situacao_servidor,
                s.co_cargo_emprego

        FROM gestaopessoa.cessaoprorrogacao cp

        LEFT JOIN gestaopessoa.orgaocessionario o ON o.ogcid = cp.ogcid

        LEFT JOIN siape.tb_servidor_simec AS s ON cast(s.nu_matricula_siape as character(7)) = cp.nu_matricula_siape --AND co_situacao_servidor IN ( 1,8,11,43 )

        WHERE cp.cprid = {$cprid}
    ";
    $dados = $db->pegaLinha($sql);
    return $dados;
}

# - deletaProcessoCessaoProrrogacao: DELETA DADOS DO CADASTRO DE CESSÃO E PRORROGAÇÃO - USADO PELAS: TELAS CADASTRO DE CESSÃO E PRORROGAÇÃO". EXCLUSÃO LOGICA. (ATUALIZA O STATUS PARA INATIVO - "I")
function deletaProcessoCessaoProrrogacao( $dados ){
    global $db;

    $tipo  = trim($dados['tipo']); #DEFINE DE ONDE VERM A REQUISIÇÃO PARA A EXCLUSÃO, SE "C" CESSÃO, SE "P" PRORROGAÇÃO E SE "S" CONSULTA.

    $cprid = trim($dados['cprid']);
    $siape = trim($dados['siape']);
    $orcid = trim($dados['orcid']);

    #VERIFICA SE A PRORROGAÇÃO PARA A DETERMINADA CESSÃO, CASSO HAJA NÃO É POSSIVÉL A EXCLUSÃO.
    if($tipo == 'C'){
        $sql_busca = "
            SELECT  count(cprid) as num_prorr
            FROM gestaopessoa.cessaoprorrogacao cp
            WHERE cp.cprstatus = 'A' AND cprtipo IN ('P', 'S') AND cp.nu_matricula_siape = '{$siape}' AND orcid = {$orcid}
        ";
        $total = $db->pegaUm($sql_busca);
    }else{
        $total = 0;
    }

    $sql = "
        UPDATE gestaopessoa.cessaoprorrogacao
            SET cprstatus = 'I'
        WHERE cprid = {$cprid};
    ";

    #CASO O RESULTADO DA CONSULTA SEJA IGUAL A 0 (NÃO A PRORROGAÇÃO PARA A CESSAO), É FEITA A EXCLUSÃO LOGICA.
    if($total == 0){
        if ( $db->executar($sql) ) {
            $db->commit();
            die('ok');
        } else {
            die('erro');
        }
    }else{
        die('usado');
    }
}

# - editarPortaria: BUSCA DADOS DA PORTARIA PARA EDIÇÃO - USADO PELAS: TELAS CADASTRO DA PORTARIA.
function editarParecer( $dados ){
    global $db;

    $prcid = $dados['prcid'];

    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    $sql = "
        SELECT  pc.prcid,
                pc.usucpf,
                u.usunome,
                pc.cprid,
                pc.prcdsc,
                pc.prcsitworkflow,
                to_char(pc.prcdtinclusao, 'DD/MM/YYYY') as prcdtinclusao,
                prcstatus
        FROM gestaopessoa.parecerchecklist pc
        JOIN seguranca.usuario AS u ON u.usucpf = pc.usucpf
        WHERE prcid = {$prcid}
    ";
    $dados = $db->pegaLinha($sql);

    if( (trim($dados['usucpf']) == trim($_SESSION['usucpf'])) || in_array(PERFIL_SUPER_USER, $perfil) ){
        $dados['permissao'] = 'S';

        $dados["usunome"] = iconv("ISO-8859-1", "UTF-8", $dados["usunome"]);
        $dados["prcdsc"] = iconv("ISO-8859-1", "UTF-8", $dados["prcdsc"]);
        $dados["prcsitworkflow"] = iconv("ISO-8859-1", "UTF-8", $dados["prcsitworkflow"]);

    }else{
        $dados['permissao'] = 'N';
    }
    echo simec_json_encode($dados);
    die();
}


# - excluirDocAnexo: TELA CADASTRO DE CESSÃO - DELETA OS DOCUMENTOS ANEXADOS.
function excluirDocAnexo( $dados ) {
    global $db;

    $arqid = $dados['arqid'];

    if ($arqid != '') {
        $sql = " UPDATE gestaopessoa.arqcessao SET aqcstatus = 'I' WHERE arqid = {$arqid} ";
    }

    if( $db->executar($sql) ){
        $db->commit();
        $_SESSION['gestao']['cprid'] = $dados['cprid'];
        $db->sucesso('principal/cessao_prorrogacao/cad_documentos', '&acao=A');
    }
}

# - excluirPortariaAnexo: TELA CADASTRO DE CESSÃO - DELETA OS PORTARIAS ANEXADA.
function excluirPortariaAnexo( $dados ) {
    global $db;

    $arqid = $dados['arqid'];

    if ($arqid != '') {
        $sql = " UPDATE gestaopessoa.docportaria SET dpcstatus = 'I' WHERE arqid = {$arqid} ";
    }

    if( $db->executar($sql) ){
        $db->commit();
        $_SESSION['gestao']['cprid'] = $dados['cprid'];
        $db->sucesso('principal/cessao_prorrogacao/cad_dados_portaria', '&acao=A');
    }
}

# - monta_abas_cessao_prorrogacao: MONTA ABAS - FORÇA DE TRABALHO CESSÃO/PRORROGAÇÃO.
function monta_abas_cessao_prorrogacao() {
    $abas[] = array("id" => 1, "descricao" => "Cessão", "link" => "/gestaopessoa/gestaopessoa.php?modulo=principal/cessao_prorrogacao/inicio_cessao_prorrogacao&acao=A&aba=cessao");
    $abas[] = array("id" => 2, "descricao" => "Prorrogação", "link" => "/gestaopessoa/gestaopessoa.php?modulo=principal/cessao_prorrogacao/inicio_cessao_prorrogacao&acao=A&aba=pror");
    $abas[] = array("id" => 3, "descricao" => "Consulta Cessão/Prorrogação", "link" => "/gestaopessoa/gestaopessoa.php?modulo=principal/cessao_prorrogacao/inicio_cessao_prorrogacao&acao=A&aba=cons");
    return $abas;
}

# - salvarDadosCessao: SALVA DADOS DO PROCESSO - CESSÃO - TELA DE CADASTRO DO PROCESSO.
function salvarDadosCessao($dados){
    global $db;

    $cprid                  = trim($dados['cprid']);
    $nu_matricula_siape     = trim($dados['nu_matricula_siape']);
    $orcid                  = $dados['orcid'];
    $cprnumprocesso         = trim($dados['cprnumprocesso']);
    $tpsid                  = $dados['tpsid'];
    $ogcid                  = $dados['ogcid'];
    $cprfuncaodesempenhada  = trim( addslashes( $dados['cprfuncaodesempenhada'] ) );
    $cprcodigosimbolo       = trim($dados['cprcodigosimbolo']);
    $tonid                  = $dados['tonid'];
    $cprsitestagio          = $dados['cprsitestagio'] == 'S' ? 't' : 'f';
    $cprsindpad             = $dados['cprsindpad'] == 'S' ? 't' : 'f';
    $cprsitdedicacao        = $dados['cprsitdedicacao'] == 'S' ? 't' : 'f';
    $cprdtinlusao           = formataDataBanco( $dados['cprdtinlusao'] );
    $aimid                  = $dados['aimid'];

    $columncprprazo = $dados['columncprprazo'];

    if($columncprprazo == 'I'){
        $columncprprazoind = 'TRUE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'U'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'TRUE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'D'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= "'".formataDataBanco( $dados['columncprprazodata'] )."'";#DATA PRAZO.
    }

    $usucpf = $_SESSION['usucpf'];#Usuário logado que insere as informações.

    unset($_SESSION['gestao']['cprid']);

    if( $cprid == '' ){
        $sql = "
            INSERT INTO gestaopessoa.cessaoprorrogacao(
                            cprtipo,
                            orcid,
                            cprnumprocesso,
                            tpsid,
                            ogcid,
                            cprfuncaodesempenhada,
                            cprcodigosimbolo,
                            tonid,
                            cprsitestagio,
                            cprsindpad,
                            cprsitdedicacao,
                            aimid,
                            cprdtinlusao,
                            usucpf,
                            nu_matricula_siape,
                            columncprprazoind,
                            columncprprazoano,
                            columncprprazodata,
                            cprstatus
                    ) VALUES (
                            'C',
                            $orcid,
                            '{$cprnumprocesso}',
                            $tpsid,
                            $ogcid,
                            '{$cprfuncaodesempenhada}',
                            '{$cprcodigosimbolo}',
                            $tonid,
                            '{$cprsitestagio}',
                            '{$cprsindpad}',
                            '{$cprsitdedicacao}',
                            $aimid,
                            '{$cprdtinlusao}',
                            '{$usucpf}',
                            '{$nu_matricula_siape}',
                            {$columncprprazoind},
                            {$columncprprazoano},
                            {$columncprprazodata},
                            'A'
                    )RETURNING cprid;
        ";
        $controle = 'I';
    }else{
        $sql = "
            UPDATE gestaopessoa.cessaoprorrogacao
                        SET orcid                   = {$orcid},
                            cprnumprocesso          = '{$cprnumprocesso}',
                            tpsid                   = {$tpsid},
                            ogcid                   = {$ogcid},
                            cprfuncaodesempenhada   = '{$cprfuncaodesempenhada}',
                            cprcodigosimbolo        = '{$cprcodigosimbolo}',
                            tonid                   = {$tonid},
                            cprsitestagio           = '{$cprsitestagio}',
                            cprsindpad              = '{$cprsindpad}',
                            cprsitdedicacao         = '{$cprsitdedicacao}',
                            aimid                   = {$aimid},
                            cprdtinlusao            = '{$cprdtinlusao}',
                            columncprprazoind       = {$columncprprazoind},
                            columncprprazoano       = {$columncprprazoano},
                            columncprprazodata      = {$columncprprazodata},
                            usucpf                  = '{$usucpf}',
                            nu_matricula_siape      = '{$nu_matricula_siape}'
                WHERE cprid = {$cprid} RETURNING cprid;
        ";
        $controle = 'U';
    }
    $dado = $db->pegaLinha($sql);

    if( $dado > 0 ){
        $db->commit();
        criaDocidCessaoProrrogacao( $dado['cprid'], $tpsid, $orcid );

        if( $controle == 'I' ){
            $_SESSION['gestao']['cprid'] = $dado['cprid'];
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_documentos', '&cprid='.$_SESSION['gestao']['cprid']);
        }elseif( $controle == 'U' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_cessao', '&cprid='.$_SESSION['gestao']['cprid']);
        }
    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_processo');
    }
}

# - salvarDadosConsulta: SALVA DADOS DO PROCESSO - CONSULTA CESSÃO/PROROGAÇÃO - TELA DE CADASTRO DA CONSULTA.
function salvarDadosConsulta($dados){
    global $db;

    $cprid                  = trim($dados['cprid']);
    $nu_matricula_siape     = trim($dados['nu_matricula_siape']);
    $orcid                  = $dados['orcid'];
    $cprnumprocesso         = trim($dados['cprnumprocesso']);
    $tpsid                  = $dados['tpsid'];
    $ogcid                  = $dados['ogcid'];
    $cprfuncaodesempenhada  = trim( addslashes( $dados['cprfuncaodesempenhada'] ) );
    $cprcodigosimbolo       = trim($dados['cprcodigosimbolo']);
    $tonid                  = $dados['tonid'];
    $cprsitestagio          = $dados['cprsitestagio'] == 'S' ? 't' : 'f';
    $cprsindpad             = $dados['cprsindpad'] == 'S' ? 't' : 'f';
    $cprsitdedicacao        = $dados['cprsitdedicacao'] == 'S' ? 't' : 'f';
    $cprdtinlusao           = formataDataBanco( $dados['cprdtinlusao'] );
    $aimid                  = $dados['aimid'];

    $columncprprazo = $dados['columncprprazo'];

    if($columncprprazo == 'I'){
        $columncprprazoind = 'TRUE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'U'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'TRUE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'D'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= "'".formataDataBanco( $dados['columncprprazodata'] )."'";#DATA PRAZO.
    }

    $usucpf = $_SESSION['usucpf'];#Usuário logado que insere as informações.

    unset($_SESSION['gestao']['cprid']);

    if( $cprid == '' ){
        $sql = "
            INSERT INTO gestaopessoa.cessaoprorrogacao(
                            cprtipo,
                            orcid,
                            cprnumprocesso,
                            tpsid,
                            ogcid,
                            cprfuncaodesempenhada,
                            cprcodigosimbolo,
                            tonid,
                            cprsitestagio,
                            cprsindpad,
                            cprsitdedicacao,
                            aimid,
                            cprdtinlusao,
                            usucpf,
                            nu_matricula_siape,
                            columncprprazoind,
                            columncprprazoano,
                            columncprprazodata,
                            cprstatus
                    ) VALUES (
                            'S',
                            '{$fdpcpf}',
                            $orcid,
                            '{$cprnumprocesso}',
                            $tpsid,
                            $ogcid,
                            '{$cprfuncaodesempenhada}',
                            '{$cprcodigosimbolo}',
                            $tonid,
                            '{$cprsitestagio}',
                            '{$cprsindpad}',
                            '{$cprsitdedicacao}',
                            $aimid,
                            '{$cprdtinlusao}',
                            '{$usucpf}',
                            '{$nu_matricula_siape}',
                            {$columncprprazoind},
                            {$columncprprazoano},
                            {$columncprprazodata},
                            'A'
                    )RETURNING cprid;
        ";
        $controle = 'I';
    }else{
        $sql = "
            UPDATE gestaopessoa.cessaoprorrogacao
                        SET orcid                   = {$orcid},
                            cprnumprocesso          = '{$cprnumprocesso}',
                            tpsid                   = {$tpsid},
                            ogcid                   = {$ogcid},
                            cprfuncaodesempenhada   = '{$cprfuncaodesempenhada}',
                            cprcodigosimbolo        = '{$cprcodigosimbolo}',
                            tonid                   = {$tonid},
                            cprsitestagio           = '{$cprsitestagio}',
                            cprsindpad              = '{$cprsindpad}',
                            cprsitdedicacao         = '{$cprsitdedicacao}',
                            aimid                   = {$aimid},
                            cprdtinlusao            = '{$cprdtinlusao}',
                            columncprprazoind       = {$columncprprazoind},
                            columncprprazoano       = {$columncprprazoano},
                            columncprprazodata      = {$columncprprazodata},
                            usucpf                  = '{$usucpf}',
                            nu_matricula_siape      = '{$nu_matricula_siape}'
                WHERE cprid = {$cprid} RETURNING cprid;
        ";
        $controle = 'U';
    }
    $dado = $db->pegaLinha($sql);

    if( $dado > 0 ){
        $db->commit();
        criaDocidCessaoProrrogacao( $dado['cprid'], $tpsid, $orcid );

        if( $controle == 'I' ){
            $_SESSION['gestao']['cprid'] = $dado['cprid'];
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_consulta', '&cprid='.$_SESSION['gestao']['cprid']);
        }elseif( $controle == 'U' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_consulta', '&cprid='.$_SESSION['gestao']['cprid']);
        }

    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_processo');
    }
}

# - salvarDadosProrrogacao: SALVA DADOS DO PROCESSO - PRORROGAÇÃO - TELA DE CADASTRO DO PROCESSO DE PRORROGAÇÃO.
function salvarDadosProrrogacao($dados){
    global $db;

    $cprid                  = trim($dados['cprid']);
    $nu_matricula_siape     = trim($dados['nu_matricula_siape']);
    $orcid                  = $dados['orcid'];
    $cprnumprocesso         = trim($dados['cprnumprocesso']);
    $tpsid                  = $dados['tpsid'];
    $ogcid                  = $dados['ogcid'];
    $cprfuncaodesempenhada  = trim( addslashes( $dados['cprfuncaodesempenhada'] ) );
    $cprcodigosimbolo       = trim($dados['cprcodigosimbolo']);
    $tonid                  = $dados['tonid'];
    $cprsitestagio          = $dados['cprsitestagio'] == 'S' ? 't' : 'f';
    $cprsindpad             = $dados['cprsindpad'] == 'S' ? 't' : 'f';
    $cprsitdedicacao        = $dados['cprsitdedicacao'] == 'S' ? 't' : 'f';

    $cprsitmudanca          = $dados['cprsitmudanca'] == 'S' ? 't' : 'f';
    $cprsitperdaprazo       = $dados['cprsitperdaprazo'] == 'S' ? 't' : 'f';
    $cprdtvencprorrogacao   = formataDataBanco( $dados['cprdtvencprorrogacao'] );
    $aimid                  = $dados['aimid'];

    $cprdtinlusao           = formataDataBanco( $dados['cprdtinlusao'] );

    $columncprprazo = $dados['columncprprazo'];

    if($columncprprazo == 'I'){
        $columncprprazoind = 'TRUE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'U'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'TRUE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'D'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= "'".formataDataBanco( $dados['columncprprazodata'] )."'";#DATA PRAZO.
    }

    $usucpf = $_SESSION['usucpf'];#Usuário logado que insere as informações.

    unset($_SESSION['gestao']['cprid']);

    if( $cprid == '' ){
        $sql = "
            INSERT INTO gestaopessoa.cessaoprorrogacao(
                            cprtipo,
                            orcid,
                            cprnumprocesso,
                            tpsid,
                            ogcid,
                            cprfuncaodesempenhada,
                            cprcodigosimbolo,
                            tonid,
                            cprsitestagio,
                            cprsindpad,
                            cprsitdedicacao,
                            cprsitmudanca,
                            cprsitperdaprazo,
                            cprdtvencprorrogacao,
                            aimid,
                            cprdtinlusao,
                            usucpf,
                            nu_matricula_siape,
                            columncprprazoind,
                            columncprprazoano,
                            columncprprazodata,
                            cprstatus
                    ) VALUES (
                            'P',
                            $orcid,
                            '{$cprnumprocesso}',
                            $tpsid,
                            $ogcid,
                            '{$cprfuncaodesempenhada}',
                            '{$cprcodigosimbolo}',
                            $tonid,
                            '{$cprsitestagio}',
                            '{$cprsindpad}',
                            '{$cprsitdedicacao}',
                            '{$cprsitmudanca}',
                            '{$cprsitperdaprazo}',
                            '{$cprdtvencprorrogacao}',
                            $aimid,
                            '{$cprdtinlusao}',
                            '{$usucpf}',
                            '{$nu_matricula_siape}',
                            {$columncprprazoind},
                            {$columncprprazoano},
                            {$columncprprazodata},
                            'A'
                    )RETURNING cprid;
        ";
        $controle = 'I';
    }else{
        $sql = "
            UPDATE gestaopessoa.cessaoprorrogacao
                        SET orcid                   = {$orcid},
                            cprnumprocesso          = '{$cprnumprocesso}',
                            tpsid                   = {$tpsid},
                            ogcid                   = {$ogcid},
                            cprfuncaodesempenhada   = '{$cprfuncaodesempenhada}',
                            cprcodigosimbolo        = '{$cprcodigosimbolo}',
                            tonid                   = {$tonid},
                            cprsitestagio           = '{$cprsitestagio}',
                            cprsindpad              = '{$cprsindpad}',
                            cprsitdedicacao         = '{$cprsitdedicacao}',
                            cprsitmudanca           = '{$cprsitmudanca}',
                            cprsitperdaprazo        = '{$cprsitperdaprazo}',
                            cprdtvencprorrogacao    = '{$cprdtvencprorrogacao}',
                            aimid                   = {$aimid},
                            cprdtinlusao            = '{$cprdtinlusao}',
                            columncprprazoind       = {$columncprprazoind},
                            columncprprazoano       = {$columncprprazoano},
                            columncprprazodata      = {$columncprprazodata},
                            usucpf                  = '{$usucpf}',
                            nu_matricula_siape      = '{$nu_matricula_siape}'
                WHERE cprid = {$cprid} RETURNING cprid;
        ";
        $controle = 'U';
    }
    $dado = $db->pegaLinha($sql);

    if( $dado > 0 ){
        $db->commit();
        criaDocidCessaoProrrogacao( $dado['cprid'], $tpsid, $orcid );

        if( $controle == 'I' ){
            $_SESSION['gestao']['cprid'] = $dado['cprid'];
            $db->sucesso( 'principal/cessao_prorrogacao/cad_dados_prorrogacao', '&acao=A&cprid='.$_SESSION['gestao']['cprid'] );
        }elseif( $controle == 'U' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_prorrogacao', '&acao=A&cprid='.$_SESSION['gestao']['cprid']);
        }

    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_processo');
    }
}

# - salvarDadosRetificacao: SALVA DADOS DO PROCESSO - RETIFICAÇÃO - TELA DE CADASTRO DA RETIFICAÇÃO.
function salvarDadosRetificacao( $dados ){
    global $db;

    $cprid                  = trim($dados['cprid']);
    $nu_matricula_siape     = trim($dados['nu_matricula_siape']);
    $orcid                  = $dados['orcid'];
    $cprnumprocesso         = trim($dados['cprnumprocesso']);
    $tpsid                  = $dados['tpsid'];
    $ogcid                  = $dados['ogcid'];
    $cprfuncaodesempenhada  = trim( addslashes( $dados['cprfuncaodesempenhada'] ) );
    $cprcodigosimbolo       = trim($dados['cprcodigosimbolo']);
    $tonid                  = $dados['tonid'];
    $cprsitestagio          = $dados['cprsitestagio'] == 'S' ? 't' : 'f';
    $cprsindpad             = $dados['cprsindpad'] == 'S' ? 't' : 'f';
    $cprsitdedicacao        = $dados['cprsitdedicacao'] == 'S' ? 't' : 'f';
    $aimid                  = $dados['aimid'];
    $cprdtinlusao           = formataDataBanco( $dados['cprdtinlusao'] );

    $columncprprazo = $dados['columncprprazo'];

    if($columncprprazo == 'I'){
        $columncprprazoind = 'TRUE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'U'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'TRUE';
        $columncprprazodata= 'NULL';
    }elseif($columncprprazo == 'D'){
        $columncprprazoind = 'FALSE';
        $columncprprazoano = 'FALSE';
        $columncprprazodata= "'".formataDataBanco( $dados['columncprprazodata'] )."'";#DATA PRAZO.
    }

    $usucpf = $_SESSION['usucpf'];#Usuário logado que insere as informações.

    unset($_SESSION['gestao']['cprid']);

    if( $cprid == '' ){
        $sql = "
            INSERT INTO gestaopessoa.cessaoprorrogacao(
                            cprtipo,
                            orcid,
                            cprnumprocesso,
                            tpsid,
                            ogcid,
                            cprfuncaodesempenhada,
                            cprcodigosimbolo,
                            tonid,
                            cprsitestagio,
                            cprsindpad,
                            cprsitdedicacao,
                            aimid,
                            cprdtinlusao,
                            usucpf,
                            nu_matricula_siape,
                            columncprprazoind,
                            columncprprazoano,
                            columncprprazodata,
                            cprstatus
                    ) VALUES (
                            'R',
                            $orcid,
                            '{$cprnumprocesso}',
                            $tpsid,
                            $ogcid,
                            '{$cprfuncaodesempenhada}',
                            '{$cprcodigosimbolo}',
                            $tonid,
                            '{$cprsitestagio}',
                            '{$cprsindpad}',
                            '{$cprsitdedicacao}',
                            $aimid,
                            '{$cprdtinlusao}',
                            '{$usucpf}',
                            '{$nu_matricula_siape}',
                            {$columncprprazoind},
                            {$columncprprazoano},
                            {$columncprprazodata},
                            'A'
                    )RETURNING cprid;
        ";
        $controle = 'I';
    }else{
        $sql = "
            UPDATE gestaopessoa.cessaoprorrogacao
                        SET orcid                   = {$orcid},
                            cprnumprocesso          = '{$cprnumprocesso}',
                            tpsid                   = {$tpsid},
                            ogcid                   = {$ogcid},
                            cprfuncaodesempenhada   = '{$cprfuncaodesempenhada}',
                            cprcodigosimbolo        = '{$cprcodigosimbolo}',
                            tonid                   = {$tonid},
                            cprsitestagio           = '{$cprsitestagio}',
                            cprsindpad              = '{$cprsindpad}',
                            cprsitdedicacao         = '{$cprsitdedicacao}',
                            aimid                   = $aimid,
                            cprdtinlusao            = '{$cprdtinlusao}',
                            columncprprazoind       = {$columncprprazoind},
                            columncprprazoano       = {$columncprprazoano},
                            columncprprazodata      = {$columncprprazodata},
                            usucpf                  = '{$usucpf}',
                            nu_matricula_siape      = '{$nu_matricula_siape}'
                WHERE cprid = {$cprid} RETURNING cprid;
        ";
        $controle = 'U';
    }
    $dado = $db->pegaLinha($sql);

    if( $dado > 0 ){
        $db->commit();
        criaDocidCessaoProrrogacao( $dado['cprid'], $tpsid, $orcid );

        if( $controle == 'I' ){
            $_SESSION['gestao']['cprid'] = $dado['cprid'];
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_retificacao', '&cprid='.$_SESSION['gestao']['cprid']);
        }elseif( $controle == 'U' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_retificacao', '&cprid='.$_SESSION['gestao']['cprid']);
        }

    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_retificacao');
    }
}


# - salvarParecer: SALVA DADOS DO PARECER - CESSÃO - TELA DE CADASTRO DO PARECER.
function salvarParecer($dados){
    global $db;

    $prcid          = $dados['prcid'];
    $usucpf         = $_SESSION['usucpf'];#Usuário logado que insere as informações.
    $cprid          = trim($dados['cprid']);
    $prcdsc         = trim( addslashes( $dados['prcdsc'] ) );
    $prcsitworkflow = trim($dados['prcsitworkflow']);

    unset($_SESSION['gestao']['cprid']);

    if( $prcid == '' ){
        $sql = "
            INSERT INTO gestaopessoa.parecerchecklist(
                            usucpf,
                            cprid,
                            prcdsc,
                            prcsitworkflow,
                            prcdtinclusao,
                            prcstatus
                    ) VALUES (
                            '{$usucpf}',
                            $cprid,
                            '{$prcdsc}',
                            '{$prcsitworkflow}',
                            'now',
                            'A'
                    )RETURNING prcid;
        ";
        $controle = 'I';
    }else{
        $sql = "
            UPDATE gestaopessoa.parecerchecklist
                        SET prcdsc = '{$prcdsc}'
                WHERE prcid = {$prcid} RETURNING prcid;
        ";
        $controle = 'U';
    }
    $dado = $db->pegaLinha($sql);

    if( $dado > 0 ){
        $db->commit();

        if( $controle == 'I' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_parecer', '&acao=A');
        }elseif( $controle == 'U' ){
            $_SESSION['gestao']['cprid'] = $cprid;
            $db->sucesso('principal/cessao_prorrogacao/cad_dados_parecer', '&acao=A');
        }

    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_parecer&acao=A');
    }

}

# - salvarParecerUni: SALVA DADOS DO PARECER REFERENTE AS UNIDADES - CESSÃO - TELA DE CADASTRO DO PARECER.
function salvarParecerUni($dados){
    global $db;

    $prcid                  = $dados['prcid'];
    #CESSÃO
    $cprid                  = $_REQUEST['cprid'];
    #TIPO DE PARECER: PODENDO SER ELES ABAIXO LISTADOS:
    $tipo                   = $dados['tipo'];
    #MOVIMENTAÇÃO
    $cprsitmovimento        = $dados['cprsitmovimento'] == 'N' ? 'FALSE' : 'TRUE';
    $usucpfsitmovimentacao  = $dados['usucpfsitmovimentacao'];
    #CAP
    $cprsitcap              = $dados['cprsitcap'] == 'N' ? 'FALSE' : 'TRUE';
    $usucpfsitcap           = $dados['usucpfsitcap'];
    #CGGP
    $cprsitcggp             = $dados['cprsitcggp'] == 'N' ? 'FALSE' : 'TRUE';
    $usucpfsitcggp          = $dados['usucpfsitcggp'];
    #COORDENAÇÃO
    $coonid                 = $dados['coonid'];
    $advid                  = $dados['advid'];
    $cprsitcoordenacao      = $dados['cprsitcoordenacao'] == 'N' ? 'FALSE' : 'TRUE';
    $usucpfsitcoordenador   = $dados['usucpfsitcoordenador'];
    #ADVOGADO
    $cprsitadvogado         = $dados['cprsitadvogado'] == 'N' ? 'FALSE' : 'TRUE';
    $usucpfsitadvogado      = $dados['usucpfsitadvogado'];

    switch ( $tipo ) {
        case 'M':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET cprsitmovimento         = {$cprsitmovimento},
                            usucpfsitmovimentacao   = '{$usucpfsitmovimentacao}'
                    WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
        case 'C':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET cprsitcap       = $cprsitcap,
                            usucpfsitcap    = '{$usucpfsitcap}'
                    WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
        case 'G':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET cprsitcggp      = $cprsitcggp,
                            usucpfsitcggp   = '{$usucpfsitcggp}'
                    WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
        case 'V':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET advid   = {$advid},
                            coonid  = {$coonid}
                    WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
        case 'O':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET cprsitcoordenacao   = {$cprsitcoordenacao},
                            usucpfsitcoordenador= '{$usucpfsitcoordenador}'
                     WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
        case 'A':
            if( $cprid != '' ){
                $sql = "
                    UPDATE gestaopessoa.cessaoprorrogacao
                        SET cprsitadvogado      = {$cprsitadvogado},
                            usucpfsitadvogado   = '{$usucpfsitadvogado}'
                     WHERE cprid = {$cprid} RETURNING cprid;
                ";
            }
            break;
    }
    if($sql != ''){
        $dado = $db->pegaLinha($sql);
    }

    if( $dado > 0 ){
        $db->commit();
        $_SESSION['gestao']['cprid'] = $cprid;
        $db->sucesso('principal/cessao_prorrogacao/cad_dados_parecer', '&cprid='.$cprid);
    }else{
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_parecer&acao=A');
    }

}

# - verificarDuplicidadeCPF: VERIFICA SE EXISTE OU SE O CPF ESTA EM DUPLICIDADE NA TABELA, ESTA FUNÇÃO É USADO EM TODAS AS TELAS - MODULO CESSÃO/PRORROGAÇÃO.
function verificarDuplicidadeCPF($dados){
    global $db;

    $nu_cpf = trim( str_replace( ".", "", str_replace( "-", "", $dados['nu_cpf'] ) ) );

    $sql = "
        SELECT COUNT(nu_cpf) qtd_cpf
        FROM siape.tb_servidor_simec s
        WHERE nu_cpf = '{$nu_cpf}'ORDER BY 1
    ";
    $dados = $db->pegaLinha($sql);
    echo simec_json_encode($dados);
    die();
}

# - verificaSeUsuarioTemChefia: VERIFICA SE EXISTE CHEFIA PARA O USUÁRIO.
function verificaSeUsuarioTemChefia( $cpfavaliado ){
    global $db;

    $sql = "
        SELECT sercpfchefe FROM gestaopessoa.servidor WHERE sercpf = '{$cpfavaliado}' AND seranoreferencia = {$_SESSION['exercicio']}
    ";
    $sercpfchefe = $db->pegaUm($sql);

    if( $sercpfchefe == '' ){
        $db->sucesso( 'principal/avaliacao_servidor/lista_grid_pess_avaliados', '', " Não é possivél realizar a avaliação, não foi atribuido chefia a esse usuário!\\n Entre em contato com: CEFAP/CGGP/SAA/MEC \\n Telefone: 61-2022-7369 ou 61-2022-7353");
    }
}

#------------------------------------- FUNÇÕES PERMISSÃO - SISTEMA GESTÃO PESSOA - SEGURANÇA MODULO CESSÃO/PRORROGAÇÃO SERVIDOR -----------------------------#

function permissaoTelaCadCessao(){
    global $db;

    $perfil = pegaPerfilGeral();

    $permissao = array();

    if( in_array( PERFIL_CESSAO_CONJUR_ADVOGADO, $perfil ) || in_array( PERFIL_CESSAO_GABINTE_MIN_ADM, $perfil ) ){
        $permissao['hab'] = "N";
        $permissao['dis'] = "disabled=\"disabled\"";
        $permissao['fal'] = TRUE;
    }else{
        $permissao['hab'] = "S";
        $permissao['dis'] = "";
        $permissao['fal'] = FALSE;
    }
    return $permissao;
}

function permissaoBotaoSalvarParecer(){
    global $db;

    $perfil = pegaPerfilGeral();

    $permissao = array();

    if( in_array( PERFIL_CESSAO_MOVIMENTACAO, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['mov'] = "onclick=\"salvarParecerUni('M');\"";
    }else{
        $permissao['mov'] = "disabled=\"disabled\"";
    }

    if( in_array( PERFIL_CESSAO_CAP, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['cap'] = "onclick=\"salvarParecerUni('C');\"";
    }else{
        $permissao['cap'] = "disabled=\"disabled\"";
    }

    if( in_array( PERFIL_CESSAO_CGGP, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['cggp'] = "onclick=\"salvarParecerUni('G');\"";
    }else{
        $permissao['cggp'] = "disabled=\"disabled\"";
    }

    if( in_array( PERFIL_CESSAO_CONJUR_ADVOGADO, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['adv'] = "onclick=\"salvarParecerUni('A');\"";
    }else{
        $permissao['adv'] = "disabled=\"disabled\"";
    }

    if( in_array( PERFIL_CESSAO_CONJUR_COORDENACAO, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['coo'] = "onclick=\"salvarParecerUni('O');\"";
    }else{
        $permissao['coo'] = "disabled=\"disabled\"";
    }

    if( in_array( PERFIL_CESSAO_CONJUR_COORDENACAO, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ){
        $permissao['adCoo'] = "onclick=\"salvarParecerUni('V');\"";
    }else{
        $permissao['adCoo'] = "disabled=\"disabled\"";
    }

    return $permissao;
}

#--------------------------------------------- FUNÇÕES WORKFLOW MODULO FORÇA DE TRABALHO CESSÃO/PRORROGAÇÃO SERVIDOR ----------------------------------#

#REGRAS WORKFLOW - BUSCA DOCID VERIFICA SE O DOCUENTO JÁ EXISTE.
function buscarDocidCessaoProrrogacao( $cprid ){
    global $db;

    $sql = "
            SELECT  cprid,
                    docid
            FROM gestaopessoa.cessaoprorrogacao
            WHERE cprid = '".$cprid."'
    ";
    $dados = $db->pegaLinha($sql);
    return $dados['docid'];
}

#REGRAS WORKFLOW - CRIA O DOCUMENTO CASO NÃO EXISTA.
function criaDocidCessaoProrrogacao( $cprid, $tpsid, $orcid ){
    global $db;

    require_once APPRAIZ ."includes/workflow.php";

    $usucpf = $_SESSION['usucpf'];

    $existeDocid = buscarDocidCessaoProrrogacao( $cprid );

    if($existeDocid == ''){
        if( $tpsid == 1 && $orcid == 1 ){
            $tpdid = WF_CESSAO_FLUXO_1;
        }
        if($tpsid == 2 && $orcid == 1){
            $tpdid = WF_CESSAO_FLUXO_2;
        }
        if($tpsid == 2 && $orcid == 3){
            $tpdid = WF_CESSAO_FLUXO_3;
        }
        if($tpsid == 3 && $orcid == 2){
            $tpdid = WF_CESSAO_FLUXO_4;
        }

        if($cprid != ''){
            $docid = wf_cadastrarDocumento($tpdid, 'Getão de Pessoas - Cessão');

            $sql = "
                UPDATE gestaopessoa.cessaoprorrogacao SET usucpf = {$usucpf}, docid = {$docid} WHERE cprid = {$cprid};
            ";

            if( $db->executar($sql) ){
                $db->commit();
            }else{
                $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cessao_prorrogacao/cad_dados_cessao', '&acao=A');
            }
        }
    }else{
        return false;
    }
}

#PEGA ESTADO ATUAL DO DOCUMENTO DO WORKFLOW.
function pegaEstadoAtualWorkflowCessao($docid){
    global $db;

    if($docid) {
        $docid = (integer) $docid;
        $sql = "
            SELECT  ed.esdid, ed.esddsc
            FROM workflow.documento d
            JOIN workflow.estadodocumento AS ed ON ed.esdid = d.esdid
            WHERE d.docid = $docid
        ";
        $estado = $db->pegaLinha($sql);
        return $estado;
    } else {
        return false;
    }
}

#-------------------------------------- FUNÇÕES COMPLEMENTA O WORKFLOW MODULO FORÇA DE TRABALHO CESSÃO/PRORROGAÇÃO SERVIDOR ----------------------------#

#ENVIAR E-MAIL PARA OS USUÁRIOS RELACIONADOS AO SISTEMA CESSÃO EM TODOS OS MOMENTOS DO TRAMITE (WORKFLOW).
function enviarEmailTramite(){
    global $db;
    $sql_u = "
        SELECT 	DISTINCT (u.usucpf),
                u.usunome,
                u.usuemail
        FROM  seguranca.perfilusuario p
        JOIN  seguranca.usuario u on u.usucpf = p.usucpf

        WHERE p.pflcod in (".PERFIL_CESSAO_CONJUR_ADM.",".PERFIL_CESSAO_SAA_ADM.",".PERFIL_CESSAO_CGGP_ADM.",".PERFIL_CESSAO_CGGA_ADM.",".PERFIL_CESSAO_GABINTE_MIN_ADM.")

        ORDER BY 1
    ";
    $email = $db->carregar($sql_u);

    if(is_array($email)){
        foreach ($email as $value) {
            $arrrEmail[] = $value['usuemail'];
        }
    }

    $sql_c = "
        SELECT  c.cprid,
                CASE
                    WHEN c.cprtipo = 'C' THEN 'Cessão'
                    WHEN c.cprtipo = 'P' THEN 'Prorrogação'
                    WHEN c.cprtipo = 'S' THEN 'Colsulta'
                    WHEN c.cprtipo = 'R' THEN 'Retificação'
                END AS cprtipo,
                p.fdpsiape,
                p.fdpnome
        FROM gestaopessoa.cessaoprorrogacao   c
        JOIN gestaopessoa.ftdadopessoal p ON p.fdpcpf = c.fdpcpf
        WHERE cprid = {$_SESSION['gestao']['cprid']}
    ";
    $dadosProcesso = $db->pegaLinha($sql_c);

    include APPRAIZ . 'includes/workflow.php';

    $docid = buscarDocidCessaoProrrogacao( $_SESSION['gestao']['cprid'] );

    $atual = wf_pegarEstadoAtual( $docid );
    $historico = wf_pegarHistorico( $docid );

    foreach ( $historico as $item ){
        $tramite .=  '<p> - '.'Origem: '.$item['esddsc'].'<br> - Destino: '.$item['aeddscrealizada'].'<br> - Feito por: '.$item['usunome'].'<br> - Na data: '.$item['htddata']. '</p>';
        $tramite .= '<hr>';
    }
    $tramite .= 'Estado atual: '.$atual['esddsc'];

    $remetente = array("nome" => "Sistema Cessão/Prorrogação Servidor", "email" => $_SESSION['email_sistema']);
    $destinatario = $arrrEmail;
    $assunto = "Tramitação de Processo - Gestão Pessoas - SIMEC";
    $conteudo = "
        <b>Tramitação de Processo - Gestão Pessoas - SIMEC</b>
        <p>
            Processo nº: {$dadosProcesso['cprid']} <br>
            Código SIAPE: {$dadosProcesso['fdpsiape']} <br>
            Nome do Servidor: {$dadosProcesso['fdpnome']} <br>
            Tipo: {$dadosProcesso['cprtipo']} <br>
        </p>
        {$tramite}
    ";

    $enviado = enviar_email( $remetente, $destinatario, $assunto, $conteudo );

    if( $enviado ){
        return true;
    }else{
        return false;
    }
}