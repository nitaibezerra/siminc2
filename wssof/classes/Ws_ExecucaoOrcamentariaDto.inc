<?php

include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Classe de mapeamento da entidade wssof.ws_execucaoorcamentariadto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_execucaoorcamentariadto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_ExecucaoOrcamentariaDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_execucaoorcamentariadto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pago' => null,
        'projetolei' => null,
        'fonte' => null,
        'resultadoprimariolei' => null,
        'rapnaoprocessadoaliquidar' => null,
        'unidadeorcamentaria' => null,
        'iduso' => null,
        'rapapagarnaoprocessado' => null,
        'rapcanceladosprocessados' => null,
        'rappagonaoprocessado' => null,
        'origem' => null,
        'unidadegestoraresponsavel' => null,
        'dotatual' => null,
        'empliquidado' => null,
        'disponivel' => null,
        'funcao' => null,
        'bloqueadosof' => null,
        'executadoporinscricaoderap' => null,
        'tipocredito' => null,
        'resultadoprimarioatual' => null,
        'subelementodespesa' => null,
        'identificadoracompanhamentopo' => null,
        'planoorcamentario' => null,
        'anoreferencia' => null,
        'natureza' => null,
        'estataldependente' => null,
        'dotacaoantecipada' => null,
        'autorizado' => null,
        'dotinicialsiafi' => null,
        'elementodespesa' => null,
        'planointerno' => null,
        'tipoapropriacaopo' => null,
        'anoexercicio' => null,
        'bloqueadoremanejamento' => null,
        'programa' => null,
        'empenhadoaliquidar' => null,
        'rapexerciciosanteriores' => null,
        'esfera' => null,
        'tematicapo' => null,
        'indisponivel' => null,
        'modalidadeaplicacao' => null,
        'acao' => null,
        'gruponaturezadespesa' => null,
        'dotacaoinicial' => null,
        'dataultimaatualizacao' => null,
        'acompanhamentopo' => null,
        'rapinscritoprocessado' => null,
        'numeroptres' => null,
        'subfuncao' => null,
        'creditocontidosof' => null,
        'rapinscritonaoprocessado' => null,
        'localizador' => null,
        'idoc' => null,
        'rapnaoprocessadoliquidadoapagar' => null,
        'mes' => null,
        'detalheacompanhamentopo' => null,
        'rappagoprocessado' => null,
        'rapnaoprocessadobloqueado' => null,
        'rapcanceladosnaoprocessados' => null,
        'rapapagarprocessado' => null,
        'categoriaeconomica' => null,
    );

}//end Class
?>