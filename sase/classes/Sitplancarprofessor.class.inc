<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 10:23
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class Sitplancarprofessor extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.sitplancarprofessor";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('spcid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'spcid' => null,
        'spcdsc' => null,
        'spccomplano' => null,
        'spcpagapiso' => null,
        'spccumprejornada' => null,
        'spccor' => null,
        'spcstatus' => null,
        'esdid' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'spcid',
        'spcdsc',
        'spccomplano',
        'spcpagapiso',
        'spccumprejornada',
        'spccor',
        'spcstatus',
        'esdid'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'spcid',
        'spcdsc',
        'spccomplano',
        'spcpagapiso',
        'spccumprejornada',
        'spccor',
        'spcstatus',
        'esdid'
    );

    protected $atributosObrigatoriosString = array();

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'spcid' => null,
        'esdid' => null
    );

    /**
     * Carrega a lista de situações cadastradas
     *
     * @param string $tipo:
     *      A - Padrão, carrega com a classe Simec_Listagem;
     *      B - Carrega utilizando a função monta_lista, da global $db;
     *
     * A ação de edição da lista utiliza a função 'editSituacao';
     */
    public function getLista($tipo = 'A')
    {
        switch($tipo){
            case 'B':
                global $db;
                $sql = <<<DML
                    select
                        '<a style=\"cursor:pointer\" onclick=\"editSituacao( ' || spcid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>' as acao,
                        spcdsc,
                        '<span style="background: ' || spccor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as spccor
                    from sase.sitplancarprofessor spc
                    order by spcid
DML;
                $cabecalho   = array('Ação', 'Situação', 'Cor');
                $alinhamento = array('center', 'left', 'center');
                $larguras    = array('5%', '90%', '5%');
                $db->monta_lista($sql,$cabecalho,30, 5,'N','','N','listaSituacao',$larguras,$alinhamento);
                break;

            case 'A':
            default:
                $sql = <<<DML
                    select
                        spcid,
                        spcdsc,
                        '<span style="background: ' || spccor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as spccor
                    from sase.sitplancarprofessor spc
                    order by spcid
DML;
                $list = new Simec_Listagem();
                $list->setQuery($sql)
                    ->setCabecalho(array('Situação', 'Cor'))
                    ->addAcao('edit', array(
                        'func' => 'editSituacao',
                        'extra-params' => array(
                            'spcid'
                        )
                    ))
                    ->setCampos(array('spcdsc', 'spccor'))
                    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                break;
        }
    }
}