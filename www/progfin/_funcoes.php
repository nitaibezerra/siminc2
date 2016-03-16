<?php
/**
 * Funções gerais do módulo.
 * $Id: _funcoes.php 102341 2015-09-11 13:20:45Z maykelbraz $
 */

/**
 * Funções comuns dos sistemas SPO.
 * @see funcoesspo.php
 */
require_once APPRAIZ . 'includes/funcoesspo.php';

/**
 * Algoritmo AES de criptografia.
 * @see Aes
 */
require_once(APPRAIZ . '/includes/Aes/aes.class');
/**
 * Algoritmo AES de criptografia.
 * @see AesCtr
 */
require_once(APPRAIZ . '/includes/Aes/aesctr.class');

/**
 * Formata um número como moeda.
 *
 * @param double $valor Valor para formatar.
 * @return text
 */
function formataDinheiro($valor)
{
    return number_format($valor, 2, ',', '.');
}

/**
 * Criptografa texto usando AES256_CBC.
 * @param string $plaintext Texto para criptografar.
 * @return string Texto criptografado.
 */
function AES256_CBC_enc($plaintext)
{
    $iv_len = 16;
    $password = '';
    $plain_text = $plaintext;

	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}

    return base64_encode(trim($plain_text));

//    return /*base64_encode(*/
//        AesCtr::encrypt($plaintext, KEY_PROGFIN, 256)
//    /*)*/;
//
////    return $plaintext;
////    $ivsize = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
////    $iv = mcrypt_create_iv($ivsize, MCRYPT_RAND);
////
////    return base64_encode(
////        $iv . mcrypt_encrypt(
////            MCRYPT_RIJNDAEL_128,
////            KEY_PROGFIN,
////            $plaintext,
////            MCRYPT_MODE_CBC,
////            $iv
////        )
////    );
}

/**
 * Decriptografa texto usando AES256_CBC.
 * @param string $ciphertext_64 Texto para decriptografar.
 * @return string Texto decriptografado.
 */
function AES256_CBC_dec($ciphertext_64)
{

    $iv_len = 16;
    $password = '';
    $enc_text = $ciphertext_64;

	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return $enc_text;

//    return AesCtr::decrypt(/*base64_decode(*/$ciphertext_64/*)*/, KEY_PROGFIN, 256);

////    return $ciphertext_64;
////    $ivsize = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
////    $ciphertext = base64_decode($ciphertext_64);
////    $iv_dec = substr($ciphertext, 0, $ivsize);
////    $ciphertext = substr($ciphertext, $ivsize);
////
////    return mcrypt_decrypt(
////        MCRYPT_RIJNDAEL_128,
////        KEY_PROGFIN,
////        $ciphertext,
////        MCRYPT_MODE_CBC,
////        $iv_dec
////    );
}

function chaveTemValor(array $lista, $chave)
{
    return isset($lista[$chave]) && !empty($lista[$chave]);
}

/* Função para montar o Relatório Dinâmico */
function montaExtratoDinamicoExtratoFolha($post) {
    global $db;
    #ver($post);
    $listagem = new Simec_Listagem();
    /* Muda o tipo do objeto  */
    if ($post['requisicao'] == 'exportarXLS') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    } else {
        $listagem->setFormFiltros('formBusca');
    }
    $cabecalho = array();
    $orderby = 1;

    /* Retorna vazio caso não seja selecionada nenhuma coluna. */
    if (count($post['dados']['cols-qualit']) == 0 || count($post['dados']['cols-qualit']) == 0) {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        $groupby_array = array();
        foreach ($post['dados']['cols-qualit'] as $valor) {
            if(trim($valor)== '') continue;

            $colunaExpressao = $db->pegaLinha("SELECT crlcod,crldsc,crlexpcallback FROM progfin.colunasextrato_fp WHERE crlexpaddgroupby = '{$valor}' AND crltipo = 'QL'");
            if(!$colunaExpressao) continue;

            /* Caso tenha função Callback */
            if ($colunaExpressao['crlexpcallback'] != '') {
                $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
            }

            /* Definindo o Order By pela coluna que possui callback de texto alinhado à esquerda. */
            if ($colunaExpressao['crlexpcallback'] == 'alinhaParaEsquerda'){
                if($orderby == 1)
                    $orderby = $valor;
                else
                    $orderby .= ','.$valor;
            }

            // Cabeçalho
            array_push($cabecalho, $colunaExpressao['crldsc']);

            // Query
            $select .= " {$colunaExpressao['crlcod']} ,";
            $groupby_array[] = $valor;
        }
        $select = substr($select, 0, strlen($select) - 1);
        $groupby = implode(',', $groupby_array);
        $groupby_array = null;
    }

    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['cols-quant'] as $valor) {
            if(trim($valor)== '') continue;

            //Adicionando Cabeçalho
            $titulo = $db->pegaLinha("SELECT rccdsccoluna as crldsc FROM progfin.regrascargacusteiofolhapagamento WHERE rccnomecoluna = '{$valor}';");
            array_push($cabecalho, $titulo['crldsc']);

            //Adicionando coluna à consulta e suas devidas mascaras e totalizadores.
            $select .= " SUM({$valor}) AS {$valor} ,";
            $listagem->addCallbackDeCampo("{$valor}", 'mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");

        }
        $select = substr($select, 0, strlen($select) - 1);
    }

    /* Filtros */
    if (count($post['dados']['filtros']) > 0) {
        foreach ($post['dados']['filtros'] as $chave => $valor) {

            $valor = array_filter($valor);

            /* @TODO  Lembrar de tratar tipo de dado depois que organizar a tabela */
            if (($valor != '') && !empty($valor)) {
                $valor = implode($valor, "','");
                $where .= " AND $chave IN ('{$valor}')";
            }
        }
    }

    /* Montando a Query */
    if ($select != '' && $groupby != '') {
        $sql = " SELECT DISTINCT {$select}
        FROM
            progfin.dadoscusteiofolhapagamento dcfp
            INNER JOIN progfin.relacaounidadesigep rus ON dcfp.dcforgao = rus.cfporgao
            INNER JOIN public.unidade uni ON uni.unicod = rus.unicod
        WHERE
            dcfp.dcfano = '{$_SESSION['exercicio']}'
        {$where}
        GROUP BY
        {$groupby}
        ORDER BY {$orderby} ";
    }

    $listagem->setQuery($sql);
    $listagem->setCabecalho($cabecalho);
    if ($post['requisicao'] != 'exportarXLS') {
        $listagem->turnOnPesquisator();
    }

    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;
    /* Imprime de acordo com a chamada */

    return $saida;
}

function formatarPedido($numPedido, $dados) {
    $pedido = str_pad($numPedido, 7, '0', STR_PAD_LEFT);
    if (!empty($dados['llfid'])) {
        $pedido .= '/' . $dados['llfid'] . ' <span class="glyphicon glyphicon-align-justify text-warning"></span>';
        $pedido = "<span style=\"white-space: nowrap;\">{$pedido}</span>";
    }

    return $pedido;
}

/* Diminuir a fonte do campo Observação */
function diminuirFonte($texto){
    return "<div style=\"font-size:10px;text-align: left\">$texto</div>";
}

function colocaIcone($esddsc, $linha) {
    switch ($linha['esdid']) {
        case ESDID_LOTE_CADASTRADO:
        case ESDID_LIBERACAO_CADASTRADO:
            return <<<HTML
<span class="glyphicon glyphicon-minus text-warning" data-toggle="popover"
      title="{$esddsc}"></span>
HTML;
        // -- no break
        case ESDID_LIBERACAO_ANALISE_SPO:
            return <<<HTML
<span class="glyphicon glyphicon-transfer text-success" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LIBERACAO_AJUSTES_UO:
            return <<<HTML
<span class="glyphicon glyphicon-transfer text-danger" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LOTE_AGUARDANDO_COMUNICACAO:
        case ESDID_LIBERACAO_AGD_COMUNICACAO:
            return <<<HTML
<span class="glyphicon glyphicon-refresh text-warning" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LOTE_ENVIADO_COM_SUCESSO:
        case ESDID_LIBERACAO_ENVIO_SUCESSO:
            return <<<HTML
<span class="glyphicon glyphicon-check text-success" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LOTE_NAO_ENVIADO:
        case ESDID_LIBERACAO_ENVIO_FALHA_AJUSTES_UO:
            return <<<HTML
<span class="glyphicon glyphicon-remove text-danger" data-toggle="popover"
      data-content="{$linha{'lfnmensagem'}}"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LOTE_PROCESSANDO:
        case ESDID_LIBERACAO_PROCESSANDO:
            return <<<HTML
<span class="glyphicon glyphicon-refresh text-warning" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        // -- no break
        case ESDID_LIBERACAO_CANCELADO:
        case ESDID_LOTE_CANCELADO:
            return <<<HTML
<span class="glyphicon glyphicon-remove" data-toggle="popover"
      title="{$esddsc}" style="cursor:pointer"></span>
HTML;
        case ESDID_LOTE_ENVIADO_PARCIALMENTE:
            if ('-' != $linha['numdocsiafi']) {
                return <<<HTML
<span class="glyphicon glyphicon-check text-success" data-toggle="popover"
      title="Enviado" style="cursor:pointer"></span>
HTML;
            } else {
                return <<<HTML
<span class="glyphicon glyphicon-remove text-danger" data-toggle="popover"
      title="Não enviado" style="cursor:pointer"></span>
HTML;
            }
        // -- no break
        default: return $linha['esdid'];
    }
}

function checkboxEnviar($lfnid, $dados) {
    $perfis = pegaPerfilGeral($_SESSION['usuario']);

    if ((ESDID_LIBERACAO_ANALISE_SPO == $dados['esdid']) && (array_intersect(
                    pegaPerfilGeral($_SESSION['usuario']), array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA)))
    ) {
        return <<<HTML
        <input type="checkbox" value="{$lfnid}" data-toggle="toggle"
               data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini" />
HTML;
    } else {
        return '<center>-</center>';
    }
}

/*
 * Transforma o campo Autorizado em Editavel
 */
function campoEditavelAutorizado($aprovado, $dados = array()) {
    $aprovado = mascaraMoeda($aprovado, false);
    if ((ESDID_LIBERACAO_ANALISE_SPO == $dados['esdid']) && (array_intersect(
                    pegaPerfilGeral($_SESSION['usuario']), array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA)))
    ) {
        $estilo = array('width'=>'120px', 'text-align'=>'right !important');

        return inputTexto('aprovado_' . $dados['lfnid2'], $aprovado, 'aprovado_' . $dados['lfnid2'], 17, true, array('return' => true, 'size' => '20', 'arrStyle'=>$estilo, 'classe'=>'somarTotalEnvio'));
    } else {
        return $aprovado;
    }
}

/**
 * Formata o estado do lote atribuíndo uma label colorida a ele.
 * @param int $estado Id do estado do lote.
 * @return string
 */
function formatarEstadoLote($estado, $descricao)
{
   switch ($estado) {
        case ESDID_LOTE_AGUARDANDO_COMUNICACAO:
            $class = 'info';
            break;
        case ESDID_LOTE_PROCESSANDO:
            $class = 'primary';
            break;
        case ESDID_LOTE_ENVIADO_COM_SUCESSO:
            $class = 'success';
            break;
        case ESDID_LOTE_ENVIADO_PARCIALMENTE:
            $class = 'warning';
            break;
        case ESDID_LOTE_NAO_ENVIADO:
            $class = 'danger';
            break;
        case ESDID_LOTE_CADASTRADO:
        default:
            $class = 'default';
    }
    if (is_array($descricao)) {
        $descricao = $descricao['esddsc'];
    }

    return <<<HTML
<span class="label label-{$class}">$descricao</span>
HTML;
}

function formatarNumeroLote($loteid)
{
    return str_pad($loteid, 7, '0', STR_PAD_LEFT);
}

function retornaCheckbox($plfid, $options) {
    return <<<HTML
    <input type="checkbox" class="controleFilho" value="{$plfid}" name="dados[plfid][{$plfid}]" class="controleFilho" />
HTML;
}

function formataCodSiafi($numdocsiafi, $linha) {
    switch ($linha['lfntransferencia']) {
        case 'S':
            return $numdocsiafi;
            // -- no break
        case 'E':
            return '<span class="glyphicon glyphicon-thumbs-down" style="color:#D9534F"></span>';
            // -- no break
        case 'C':
            return '<span class="glyphicon glyphicon-remove" style="color:gray"></span>';
    }
}