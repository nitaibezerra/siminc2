<?php
/**
 * Implementação de uma classe abstrata de fonte de dados para a Simec_Listagem.
 *
 * $Id: Datasource.php 103935 2015-10-21 16:54:47Z maykelbraz $
 * @filesource
 * @package Simec\Listagem\Datasource
 */

/**
 * Classe abstrata de fonte de dados.
 *
 * @package Simec\Listagem\Datasource
 * @see \Simec_Listagem
 */
abstract class Simec_Listagem_Datasource
{
    /**
     * Quantidade de registros por página.
     */
    const TAMANHO_PADRAO_PAGINA = 100;

    /**
     * @var mixed Fonte de dados.
     */
    protected $source = null;

    /**
     * @var int Número total de registros do datasource.
     */
    protected $totalRegistros = null;

    protected $numRegistrosPorPagina = self::TAMANHO_PADRAO_PAGINA;

    /**
     * @var int Número da página que deverá ser exibida.
     */
    protected $paginaAtual = 1;

    /**
     * @var int Número total de páginas contidas no datasource.
     */
    protected $totalPaginas;

    /**
     * Retorna a fonte de dados completa.
     *
     * @return string
     */
    public function getSource()
    {
        return $this->source;
    }

    /**
     * Define a página atualmente selecionada pelo usuário.
     *
     * @param int|string $pagina O número da página atual, ou 'all'.
     * @return \Simec_Listagem_Datasource
     */
    public function setPaginaAtual($pagina)
    {
        if (empty($pagina)) {
            $pagina = 1;
        }

        $this->paginaAtual = $pagina;
        return $this;
    }

    /**
     * Retorna a página atualmente selecionada pelo usuário.
     *
     * @return int|string
     */
    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    /**
     * Computa, se necessário, e retorna a quantidade de registros da fonte de dados.
     * @return int
     */
    public function getTotalRegistros()
    {
        if (is_null($this->totalRegistros)) {
            $this->totalRegistros = $this->contaRegistros();
        }

        return $this->totalRegistros;
    }

    /**
     * Computa, se necessário, e retorna a quantidade de páginas da fonte de dados.
     * @return int
     */
    public function getTotalPaginas()
    {
        if (is_null($this->totalPaginas)) {
            $this->totalPaginas = ceil($this->getTotalRegistros() / $this->numRegistrosPorPagina);
        }

        return $this->totalPaginas;
    }

    /**
     * Verifica se a fonte de dados tem algum registro.
     *
     * @return bool
     */
    public function estaVazio()
    {
        return 0 === (int)$this->getTotalRegistros();
    }

    /**
     * Verifica se existe mais de uma página de dados.
     *
     * @return bool
     */
    public function paginar()
    {
        return $this->getTotalRegistros() > $this->getRegistrosPorPagina();
    }

    /**
     * Calcula e retorna o offset da página selecionada.
     *
     * @return int
     */
    protected function offset()
    {
        return ($this->paginaAtual - 1) * $this->getRegistrosPorPagina();
    }

    /**
     * Retorna a quantidade de registros exibidos por página.
     * @return int
     */
    protected function getRegistrosPorPagina()
    {
        return $this->numRegistrosPorPagina;
    }

    /**
     * Define uma nova quantidade de registros por página.
     *
     * @param int $numRegistrosPorPagina Quantidade de registros por página.
     */
    public function setRegistrosPorPagina($numRegistrosPorPagina)
    {
        $this->numRegistrosPorPagina = $numRegistrosPorPagina;
    }

    /**
     * Implemente a definição de uma fonte de dados e suas opções.
     *
     * @return \Simec_Listagem_Datasource
     */
    public abstract function setSource($source, array $opcoes = array());

    /**
     * Implementa a forma de retornar a query que originou o conjunto de dados.
     *
     * @returns string
     */
    public abstract function getQuery();

    /**
     * Esta é a função de retorno de dados da página atualmente selecionada.
     * Com base no offset e número de registros a serem retornados, a implmentação esta função deve
     * retornar apenas o conjunto de dados a serem exibidos na lista atual.
     *
     * @return mixed[] Lista de dados para exibição
     */
    public abstract function getDados();

    /**
     * Descobre a quantidade total de registros.
     *
     * @return int
     */
    protected abstract function contaRegistros();
}
