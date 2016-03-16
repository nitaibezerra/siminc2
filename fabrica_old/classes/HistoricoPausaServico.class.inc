<?php


/**
 * Classe responsavel pela manipulacao da pausa dos servicos da fabrica
 * @author Silas Matheus
 * @name HistoricoPausaServico
 *
 */
class HistoricoPausaServico extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.historicopausaservico";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("hpsid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hpsid' => null,
        'obsid' => null,
        'hpsinicio' => null,
        'hpstermino' => null
    );

    public function salvar($arrDados){

    	$data = date('Y-m-d H:i:s');
        $scsid = $arrDados['scsid'];
        $odsid = $arrDados['odsid'];
        $usucpf = $_SESSION['usucpf'];
        $obsdsc = $arrDados['obsdsc'];
        $estadoPausa = $this->estadoPausa($odsid, $scsid);


        if(empty($scsid)){

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm($sqlSS);
            
        }

        
        $sqlOBS = "INSERT INTO fabrica.observacoes
                        (usucpf, obsdsc, obsstatus, scsid, obsdata, obstp)
                   VALUES
                        ('$usucpf', '$obsdsc', 'A', $scsid, '$data', 'O')
                   RETURNING
                        obsid";
        $obsid = $this->pegaUm($sqlOBS);


        enviaEmailCadSolicitacao($scsid, 'PAUSA');

        if($estadoPausa == 'I'){

            $tipo = "T";
            $this->atualizarTermino($data, $odsid, $scsid);

        }else{

            $tipo = "I";
            
        }
        
        $sqlHis = "INSERT INTO $this->stNomeTabela
                        (obsid, hpsdata, hpstipo)
                   VALUES
                        ($obsid, '$data', '$tipo')
                   RETURNING
                        hpsid";
        return $hpsid = $this->pegaUm($sqlHis);

        
    }

    public function atualizarTermino($data, $odsid, $scsid){
        if(empty($scsid)){

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm($sqlSS);

        }

        // calculando tempo da pausa
        $dataAnterior = strtotime($this->pegarInicioPausa($scsid));
        $data = strtotime($data);
        $diferenca = $data - $dataAnterior;

        $prevtermino = strtotime($this->pegarDataTermino($scsid,$odsid));
        $prevtermino = date("Y-m-d H:i:s", $prevtermino + $diferenca);

        
        $sqlOS = "UPDATE fabrica.ordemservico SET odsdtprevtermino='$prevtermino' WHERE scsid=$scsid RETURNING odsid";
        $odsid = $this->pegaUm($sqlOS);
        

        $sqlANS = "UPDATE fabrica.analisesolicitacao SET ansprevtermino='$prevtermino' WHERE scsid=$scsid RETURNING ansid";
        $ansid = $this->pegaUm($sqlANS);


    }
    public function pegarDataTermino($scsid,$odsid){

         if($odsid){
             $sql = "SELECT odsdtprevtermino FROM fabrica.ordemservico WHERE scsid=$scsid and tosid = 1";
         }else{
             $sql = "SELECT ansprevtermino FROM fabrica.analisesolicitacao WHERE scsid=$scsid";
         }
        
        return $this->pegaUm($sql);

    }

    /**
     * Pega o tipo (inico ou termino de pausa)
     * @name estadoPausa
     * @param <int> $odsid
     * @return <type>
     */
    public function estadoPausa($odsid = '', $scsid = ''){

        if(empty($scsid)){

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm($sqlSS);

        }

        $sqlSel = "SELECT
                        hps.hpstipo
                   FROM
                        $this->stNomeTabela hps
                   INNER JOIN
                        fabrica.observacoes obs
                        ON obs.obsid=hps.obsid
                   WHERE
                        obs.scsid=$scsid
                   ORDER BY
                        hps.hpsdata DESC
                   LIMIT 1";

        return $this->pegaUm($sqlSel);
        
    }

    /**
     * Pega o inicio da ultima pausa
     * @name pegarInicioPausa
     * @param <type> $odsid
     * @return <type>
     */
    public function pegarInicioPausa($scsid = ''){

        $sqlSel = "SELECT
                        hps.hpsdata
                   FROM
                        $this->stNomeTabela hps
                   INNER JOIN
                        fabrica.observacoes obs
                        ON obs.obsid=hps.obsid
                   WHERE
                        obs.scsid=$scsid
                   AND
                        hps.hpstipo='I'
                   ORDER BY
                        hps.hpsdata
                   DESC";

        return $hpsdata = $this->pegaUm($sqlSel);
        
    }

    /**
     * Monta Grid
     * @name montaGrid
     * @param <int> $odsid - Código da ordem de servico
     * @return void
     */
    public function montaGrid($odsid = '', $scsid = ''){

        if(empty($scsid)){

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm($sqlSS);

        }

        $sql = "SELECT
                    (SELECT distinct usu.usunome FROM seguranca.usuario usu 
			INNER JOIN fabrica.observacoes obs2 on obs2.usucpf = usu.usucpf
			where usu.usucpf = obs.usucpf) as usuario,
                    (to_char(hps.hpsdata, 'DD/MM/YYYY - HH24:MI:SS')) as data,
                  
        (CASE WHEN (hps.hpstipo = 'I') THEN
                        'Início'
                    ELSE
                        'Término'
                    END) tipo,
                    obs.obsdsc
               FROM
                    $this->stNomeTabela hps
               INNER JOIN
                    fabrica.observacoes obs
                    ON obs.obsid=hps.obsid
                WHERE
                    obs.scsid=$scsid
                ORDER BY
                    hps.hpsdata DESC";

        $cabecalho =  array('Autor','Data/Hora', 'Ação', 'Justificativa');
        $alinhameto =  array('center','center', 'left');
        $this->monta_lista($sql, $cabecalho, 20, 50, false, "center", 'S', '', '', $alinhameto);

    }

}