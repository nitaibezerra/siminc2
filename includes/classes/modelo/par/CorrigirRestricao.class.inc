<?php

/**
 * @author Jair Foro <jairsanto@mec.gov.br>
 */
class CorrigirRestricao extends Modelo{
    public function corrigirRestricao(){
        $arrRestricao = $this->buscarRestricao();
        if ($arrRestricao) {
            foreach ($arrRestricao as $restricao) {
                $sqlWF = "insert into workflow.documento
		( tpdid, esdid, docdsc )
		values ( " . TPDID_RESTRICAO_PAR . ", " . ESDID_EM_CADASTRAMENTO . ", 'Restrição Termo')
		returning docid";
                $docid = $this->pegaUm( $sqlWF );
                
                 $sql = "
                    INSERT INTO 
                        par.restricaoentidade(
                            resid, 
                            inuid, 
                            docid
                        )
                    VALUES(
                       " . $restricao['resid'] . ",
                       " . $restricao['inuid'] . ",
                        " . $docid . "
                    )
                ";
                $this->executar($sql);                
                
            }
            $this->commit();
        }
    }
    
   
    public function buscarRestricao() {
        $sql = "SELECT r.resid,
                        r.resdescricao,
                        ir.inuid,
                        ir.iurid,
                        iu.muncod,
                        m.mundescricao,
                        iu.estuf
                 FROM par.restricaofnde r
                      INNER JOIN par.instrumentounidaderestricaofnde ir ON ir.resid = r.resid
                      INNER JOIN par.instrumentounidade iu ON iu.inuid = ir.inuid
                      INNER JOIN territorios.municipio m ON m.muncod = iu.muncod
                 ORDER BY r.resid";

        $arrRestricao = $this->carregar($sql);        
        return $arrRestricao;
    }
}