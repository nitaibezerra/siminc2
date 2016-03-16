<?php

class Simec_MultiCadastro
{
    const K_TIPO_TELA  = 'T';
    const K_TIPO_MODAL = 'M';

    private $id;
    private $tipo;
    private $titulo = 'Cadastro';
    private $callback;
    private $listagem;
    private $formulario;
    private $actionForm;
    private $formPadrao = true;
    private $boInserir = true;

    public function __construct($dados = null, $id = null, $tipo = null)
    {
        $this->id = $id ? $id : 'multi_' . rand() . rand();
        $this->tipo = $tipo ? $tipo : self::K_TIPO_TELA;
    }

    /**
     * @return mixed
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @param mixed $id
     */
    public function setId($id)
    {
        $this->id = $id;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getTipo()
    {
        return $this->tipo;
    }

    /**
     * @param mixed $tipo
     */
    public function setTipo($tipo)
    {
        $this->tipo = $tipo;
        return $this;
    }


    /**
     * @return mixed
     */
    public function getTitulo()
    {
        return $this->titulo;
    }

    /**
     * @param mixed $titulo
     */
    public function setTitulo($titulo)
    {
        $this->titulo = $titulo;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getListagem()
    {
        return $this->listagem;
    }

    /**
     * @param mixed $listagem
     */
    public function setListagem($listagem)
    {
        $this->listagem = $listagem;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getFormulario()
    {
        return $this->formulario;
    }

    /**
     * @param mixed $formulario
     */
    public function setFormulario($formulario)
    {
        $this->formulario = $formulario;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getBoInserir()
    {
        return $this->boInserir;
    }

    /**
     * @param mixed $boInserir
     */
    public function setBoInserir($boInserir)
    {
        $this->boInserir = $boInserir;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getCallback()
    {
        return $this->callback;
    }

    /**
     * @param mixed $callback
     */
    public function setCallback($callback)
    {
        $this->callback = $callback;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getActionForm()
    {
        return $this->actionForm;
    }

    /**
     * @param mixed $actionForm
     */
    public function setActionForm($actionForm)
    {
        $this->actionForm = $actionForm;
    }

    /**
     * @return boolean
     */
    public function getFormPadrao()
    {
        return $this->formPadrao;
    }

    /**
     * @param boolean $formPadrao
     */
    public function setFormPadrao($formPadrao)
    {
        $this->formPadrao = $formPadrao;
    }
}