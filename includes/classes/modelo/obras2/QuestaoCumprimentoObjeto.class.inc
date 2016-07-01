<?php
class QuestaoCumprimentoObjeto {

    public $qcodId;
    public $qstId;
    public $qcodNumero;
    public $qcodDsc;
    public $qstObs;
    public $qcodResposta;

    public $img;
    public $imgValidacao;
    public $titulo;
    public $respostaTrue;
    public $respostaFalse;
    public $respostaNA;
    public $arquivos = array();

    public function pegaResposta($obrid, $questao)
    {
        $qcodNumero = $questao['qstnumero'];

        $validador = new Validador();
        $check = $validador->check($obrid, $questao['vdrid']);
        
        $arquivos = 0;
        if(is_array($check)) {
            $qcodResposta = $check['resposta'];
            $arquivos = count($check['arquivos']);
        } else {
            $qcodResposta = $check;
        }
        
        $qcodResposta = $this->contaArquivos($questao['arqid'],$arquivos, $qcodResposta);
        return array('numero' => $qcodNumero, 'resposta' => $qcodResposta);
    }

    public function trataQuestao($obrid,$questao)
    {
        $this->qcodId = $questao['qcodid'];
        $this->qstId = $questao['qstid'];
        $this->qcodDsc = $questao['qstdsc'];
        $this->qcodResposta = $questao['qcodresposta'];
        $this->qcodNumero = $questao['qstnumero'];
        $this->qstObs = $questao['qstobs'];

        $validador = new Validador();
        $check = $validador->check($obrid, $questao['vdrid']);
        if(is_array($check)) {
            $this->qcodResposta = $check['resposta'];
            $this->arquivos = $check['arquivos'];
        } else {
            $this->qcodResposta = $check;
        }
        $this->questao($validador->getMessage());
        $this->buscaArquivos($questao['arqid'],$questao['arqnome'],$questao['arqextensao']);
        return $this;
    }

    private function questao($mensagem)
    {
        if($this->qcodResposta == 't') {
            $this->respostaTrue = 'checked';
            $this->imgValidacao = 'valida1.gif';
        } else if($this->qcodResposta == 'f') {
            $this->respostaFalse = 'checked';
            $this->imgValidacao = 'valida5.gif';
            $this->titulo = 'title="'.$mensagem.'"';
        } else {
            $this->respostaNA = 'checked';
            $this->imgValidacao = 'valida1.gif';
        }
    }

    private function buscaArquivos($arqid,$arqnome,$arqextensao)
    {
        if(!$arqid) {
            return false;
        }
        $id = explode(',',$arqid);
        $nome = explode(',',$arqnome);
        $extensao = explode(',',$arqextensao);
        $saida = array();
        foreach($id as $key => $value) {
            $saida[] = array('arqid' => $id[$key],'arqnome' => $nome[$key],'arqextensao' => $extensao[$key],'del' => true);
        }
        $this->qcodResposta = 't';
        $this->respostaTrue = 'checked';
        $this->respostaFalse = '';
        $this->respostaNA = '';
        $this->imgValidacao = 'valida1.gif';
        $this->arquivos = array_merge($this->arquivos,$saida);
    }

    private function contaArquivos($arqid, $arquivosValidador, $resposta)
    {
        $numero = 0;
        if($arqid) {
            $numero = count(explode(',',$arqid));
        }
        if($resposta != 't' && ($numero + $arquivosValidador) > 0) {
            $resposta = 't';
        }
        return $resposta;
    }
}



