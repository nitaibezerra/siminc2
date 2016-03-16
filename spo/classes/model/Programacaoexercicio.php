<?php
/**
 * Abstração do mapeamento da entidade {esquema}.programacaoexercicio.
 *
 * $Id: Programacaoexercicio.php 100919 2015-08-06 19:01:37Z maykelbraz $
 * @filesource
 */

/**
 * Mapeamento da entidade proporc.programacaoexercicio.
 *
 * @see Modelo
 */
class Spo_Model_Programacaoexercicio extends Modelo
{
    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela;

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'prsano',
    );

    /**
     * @var string[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'prsano' => null,
        'prsdata_inicial' => null,
        'prsdata_termino' => null,
        'prsexerccorrente' => null,
        'prsstatus' => null,
        'prsativo' => null,
        'prsexercicioaberto' => null,
    );

    /**
     * Construtor da classe.
     *
     * @param string $esquema O Nome do esquema da tabela.
     * @param int|null $id O ID do registro a ser consultado.
     * @throws Exception Lançada se o esquema informado for vazio.
     */
    public function __construct($esquema, $id = null)
    {
        if (empty($esquema)) {
            throw new Exception('O valor de "$esquema" não pode ser vazio.');
        }

        $this->stNomeTabela = "{$esquema}.programacaoexercicio";
        parent::__construct($id);
    }

    public function queryCombo()
    {
        $opcoes = array('query' => true);
        return $this->recuperarTodosFormatoInput('prsano', array(), 'descricao DESC', $opcoes);
    }
}
