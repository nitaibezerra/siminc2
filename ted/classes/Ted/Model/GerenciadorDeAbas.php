<?php

/**
 * Class Ted_Model_GerenciadorDeAbas
 */
class Ted_Model_GerenciadorDeAbas
{
    /**
     * @var array
     */
    private $abaItens = array();

    /**
     * @var cls_banco
     */
    private $db;

    /**
     * @var
     */
    private $pflcod;

    /**
     * ID do sistema
     */
    const SIS_ID = 194;

    /**
     *
     */
    public function __construct()
    {
        $this->db = new cls_banco();
        $this->getPflcod();
        $this->addItensMenu();
    }

    /**
     * pega os perfis atribuidos ao menu
     * @return void(0)
     */
    public function getPflcod()
    {
        $pflcod = Ted_Utils_Model::pegaPerfilUsuario();
        if (is_array($pflcod)) {
            foreach ($pflcod as $perfil) {
                $this->pflcod .= "{$perfil['pflcod']},";
            }
            unset($pflcod);
        }
        if (strlen($this->pflcod)) {
            $this->pflcod = substr($this->pflcod, 0, -1);
        } else  {
            $this->pflcod = false;
        }
    }

    /**
     * pega os itens do menu por perfil de usuário
     * @return array|bool|void
     */
    public function getMenuItens()
    {
        if (!$this->pflcod) return false;

        $strSQL = sprintf("
            select * from seguranca.menu
            where sisid = %d and mnutipo = 2 and mnulink is not null
            and mnuidpai = 15145 and mnushow = 'f'
            and mnuid in (
                select mnuid from seguranca.perfilmenu where mnuid in (
                    select mnuid from seguranca.menu where sisid = %d and mnutipo = 2 and mnulink is not null and mnuidpai = 15145
                ) and pflcod in ({$this->pflcod}) order by pflcod asc
            )
            order by mnucod asc
        ", self::SIS_ID, self::SIS_ID);
        //ver($strSQL, d);

        return $this->db->carregar($strSQL);
    }

    /**
     * Formata o array para o methodo $this->escreveAba
     * @return void(0)
     */
    private function addItensMenu()
    {
        $collection = $this->getMenuItens();
        //ver($collection, d);
        if (is_array($collection)) {
            foreach ($collection as $row) {
                $result = preg_match('![^\/]+&!', $row['mnulink'], $matches);

                if ($result) {
                    $matches = substr($matches[0], 0, -1);
                }

                $this->abaItens[$row['mnucod']] = array(
                    'link'   => $row['mnulink'],
                    'titulo' => $row['mnudsc'],
                    'valor'  => $matches
                );
            }

            $this->aclFilter();
        }
    }

    /**
     * @return void(0)
     */
    private function aclFilter()
    {
        //Extrato Ações Detalhe, Extrato Ações
        unset($this->abaItens[4000], $this->abaItens[4001]);

        //1010 - RCO
        if (!Ted_Utils_Model::showRCO()) {
            unset($this->abaItens[1010]);
        }

        //1011 - FNDE
        if (!Ted_Utils_Model::concedenteFNDE()) {
            unset($this->abaItens[1011]);
        }

        /**
         * TODO
         * - remove quando a funcionalidade estiver pronta
         * 1012 - Programação Financeira
         */
        unset($this->abaItens[1012]);
    }

    /**
     * @return string
     */
    private function capturaTed()
    {
		return Ted_Utils_Model::capturaTcpid() ? '&ted='.Ted_Utils_Model::capturaTcpid() : '';
	}

    /**
     * @return string
     */
    public function apresentaAbas()
    {
		$textoAba  = '<div class="list-group">';
		$textoAba .= '	<a href="#" class="list-group-item " style="cursor:default;"><h4><span class="glyphicon glyphicon-th-list"></span> Abas</h4></a>';
        foreach ($this->abaItens as $item) {
            $textoAba .= $this->escreveAba($item);
        }
		$textoAba .= '</div>';
		return $textoAba;
	}

    /**
     * @param $aba
     * @return string
     */
    public function escreveAba($aba)
    {
		$abaAtiva = $this->capturaAbaAtiva();
		
		if (Ted_Utils_Model::capturaTcpid()) {
			$linha = "<a href='{$aba['link']}{$this->capturaTed()}' ";
		} else {
			$linha = '<a  ';
		}

		$classe = 'list-group-item';
		$glyphicon = '<span class="glyphicon glyphicon-arrow-left"></span>';
		
		if ($abaAtiva != null && $abaAtiva == $aba['valor']) {
			$classe .= ' active';
			$glyphicon = '<span class="glyphicon glyphicon-arrow-right"></span>';
		}

		$linha .= "class='{$classe}'>";
		$linha .= $glyphicon;
		$linha .= " {$aba['titulo']}</a>";
		return $linha;
	}

    /**
     * @return mixed
     */
    public function capturaAbaAtiva()
    {
		preg_match('@([a-z]*)&acao=A@su', $_SERVER['REQUEST_URI'],$match);
		$tab = $match[1];
		return $tab;					
	}

}
