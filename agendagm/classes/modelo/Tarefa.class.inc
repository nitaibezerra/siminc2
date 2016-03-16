<?php
/**
 * Classe de mapeamento da entidade agendagm.tarefa.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade agendagm.tarefa.
 *
 * @see Modelo
 */
class Agendagm_Model_Tarefa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'agendagm.tarefa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'trfid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ptaid' => array('tabela' => 'agendagm.pauta', 'pk' => 'ptaid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'trfid' => null,
        'uamid' => null,
        'trfdsc' => null,
        'usucpf' => null,
        'ptaid' => null,
        'trfprazo' => null,
        'tardtinclusao' => null,
        'tarstatus' => null,
    );

    public function excluirTarefaPauta( $trfid ){

        #BUSCA ARQUIVOS PARA EXCLUSÃO FISICA
        $sql = "SELECT arqid FROM agendagm.arquivotarefa WHERE trfid = {$trfid}";
        $arr_arqid = $this->carregarColuna($sql);

        $sql = "
            DELETE FROM agendagm.tarefausuario WHERE trfid = {$trfid};
            DELETE FROM agendagm.arquivotarefa WHERE trfid = {$trfid};
            DELETE FROM agendagm.tarefa WHERE trfid = {$trfid}
        ";
        $result = $this->pegaUm($sql);

        if( $result > 0 ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            if( is_array( $arr_arqid ) ){
                $file = new FilesSimec("arquivotarefa", array(), "agendagm");

                foreach( $arr_arqid as $arqid ){
                    $resp = $file->excluiArquivoFisico($arqid);
                }
            }
        }
    }

    public function listagemTarefasPautas( $ptaid ){
        #GERA LISTAGEM DAS TAREFAS DAS PAUTAS. EMCAMINHAMENTO.
        if( $ptaid > 0 ){
            $where = "WHERE t.ptaid = {$ptaid}";
        }else{
            $where = "WHERE 1<>1";
        }
        $sql = "
            SELECT  t.trfid,
                    t.trfdsc,
                    usunome,

                    CASE
                        WHEN (t.trfprazo::date - NOW()::date ) = '3' THEN '<p style=\"color:#1E90FF !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        WHEN (t.trfprazo::date - NOW()::date ) < '3' THEN '<p style=\"color:#FF0000 !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        ELSE '<p style=\"color:#000000 !important;\">'||to_char(t.trfprazo, 'DD/MM/YYYY')||'</p>'
                    END AS trfprazo,

                    CASE WHEN a.trfid IS NOT NULL
                        THEN 'S'
                        ELSE 'N'
                    END AS arqid

            FROM agendagm.tarefa AS t
            JOIN seguranca.usuario AS u ON u.usucpf = t.usucpf
            LEFT JOIN(
                SELECT DISTINCT trfid FROM agendagm.arquivotarefa
            ) AS a ON a.trfid = t.trfid

            {$where}

            ORDER BY 1
        ";
        $cabecalho = array("Tarefa", "Responsável", "Prazo", "Anexo");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'editaTarefaPauta');
        $listagem->addAcao('delete', 'excluirTarefaPauta');
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function listagemAnexosTarefas( $trfid ){
        #GERA LISTAGEM DOS ANEXOS DAS TAREFAS
        if( $trfid > 0 ){
            $WHERE = "WHERE t.trfid = {$trfid}";
        }else{
            $WHERE = "WHERE 1<>1";
        }

        $sql = "
            SELECT  t.arqid,
                    a.arqnome||'.'||arqextensao AS arquivo,
                    u.usunome

            FROM agendagm.arquivotarefa  AS t

            JOIN public.arquivo AS a ON a.arqid = t.arqid
            JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf

            {$WHERE}

            ORDER BY 1
        ";
        $cabecalho = array("Arquivo", "Responsável");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('download', 'downloadArquivoTarefa');
        $listagem->addAcao('delete', 'excluirArquivoTarefa');
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    
    public function listagemGridTarefas( $dados ){
        #FILTROS DE PESQUISA
        if( $dados['trfdsc'] != '' ){
            $_WHERE[] = "trfdsc ilike ('%{$dados['trfdsc']}%')";
        }

        if( $dados['ptasubtema'] != '' ){
            $_WHERE[] = "ptasubtema ilike ('%{$dados['ptasubtema']}%')";
        }

        if( $dados['trfprazo_ini'] != '' || $dados['trfprazo_fim'] != ''){
            if( $dados['trfprazo_ini'] != '' ){
                $trfprazo_ini = formata_data( $dados['trfprazo_ini'] );
            }
            if( $dados['trfprazo_fim'] != '' ){
                $trfprazo_fim = formata_data( $dados['trfprazo_fim'] );
            }

            $_WHERE[] = "( trfprazo BETWEEN '{$trfprazo_ini}' AND '{$trfprazo_fim}' )";
        }

        if( $dados['usucpf'] != '' ){
            $_WHERE[] = "t.usucpf = '{$dados['usucpf']}'";
        }

        if( $dados['uamid'] != '' ){
            $_WHERE[] = "t.uamid = {$dados['uamid']}";
        }

        if( $_WHERE[0] != '' ){
            $_WHERE = ' AND '.implode($_WHERE, ' AND ');
        }

        #GERA LISTAGEM DAS TAREFAS. EMCAMINHAMENTO.
        $sql = "
            SELECT  t.trfid,
                    t.trfdsc,
                    UPPER(und.uamdsc) AS uamdsc,
                    usunome,
                    --to_char(t.trfprazo, 'DD/MM/YYYY') as trfprazo,

                    CASE
                        WHEN (t.trfprazo::date - NOW()::date ) = '3' THEN '<p style=\"color:#1E90FF !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        WHEN (t.trfprazo::date - NOW()::date ) < '3' THEN '<p style=\"color:#FF0000 !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        ELSE '<p style=\"color:#000000 !important;\">'||to_char(t.trfprazo, 'DD/MM/YYYY')||'</p>'
                    END AS trfprazo,

                    CASE WHEN a.trfid IS NOT NULL
                        THEN 'S'
                        ELSE 'N'
                    END AS arqid

            FROM agendagm.tarefa AS t
            JOIN seguranca.usuario AS u ON u.usucpf = t.usucpf
            JOIN public.unidadeareamec AS und ON und.uamid = t.uamid
            LEFT JOIN(
                SELECT DISTINCT trfid FROM agendagm.arquivotarefa
            ) AS a ON a.trfid = t.trfid

            WHERE t.ptaid IS NULL {$_WHERE}

            ORDER BY 1
        ";
        $cabecalho = array("Pendência", "Unidade", "Responsável", "Prazo", "Anexo");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('ptadscata', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('ptatema', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('ptasubtema', 'alinhaParaEsquerda');
        $listagem->addAcao('edit', 'editarTarefa');
        $listagem->addAcao('delete', 'deletar');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);        
    }

}