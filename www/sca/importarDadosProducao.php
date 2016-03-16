<?php
ini_set("memory_limit", "1024M");

//$_REQUEST['baselogin'] = "simec_desenvolvimento_old";

// carrega as funções específicas do módulo
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$db    = new cls_banco();
$count = 10000;

// busca fotos de visitantes
$sqlVisitantes = "select vttid, im_foto_visitante, tb_imagem, img_id, img_id_cp 
          from ((select  t1.vttid
                       , t3.im_foto_visitante as im_foto_visitante
                       , 'tmp_tb_spt_visitante' as tb_imagem
                       , t3.co_visitante::text as img_id
                       , 'co_visitante' as img_id_cp
                   from sca.visitante t1
                  inner join sca.tmp_tb_spt_visitante t3
                     on t1.vttid = t3.vttidnew
                   left join sca.visitantefoto t2
                     on t1.vttid = t2.vttid
                  where t2.svfid is null
                    and t3.possuifoto = true
                    and t3.migrado = false
                  limit $count)
                  union
                (select  t1.vttid
                       , t4.im_foto_autorizado as im_foto_visitante
                       , 'tmp_tb_sca_autorizado' as tb_imagem
                       , t4.nu_cpf::text as img_id
                       , 'nu_cpf' as img_id_cp
                   from sca.visitante t1
                  inner join sca.tmp_tb_sca_autorizado t4
                     on t1.vttdoc = t4.nu_cpf
                   left join sca.visitantefoto t2
                     on t1.vttid = t2.vttid
                  where t2.svfid is null
                    and t4.possuifoto = true
                    and t4.migrado = false
                  limit $count)) as t0
         limit $count";
//exit($sqlVisitantes);
// busca fotos de servidores
$sqlServidores   = "select 
                        t1.nu_matricula_siape
                        , t3.foto
                        , 'tmp_tb_scf_cadastro_cartao' as tb_imagem
                    from sca.vwservidorativo t1
                    left join sca.visitantefoto t2
                        on t1.nu_matricula_siape = t2.nu_matricula_siape
                    left join sca.tmp_tb_scf_cadastro_cartao t3
                        on t1.nu_matricula_siape = t3.nu_matricula_siape
                        where t2.svfid is null
                        and t3.nu_matricula_siape is not null and t3.foto is not null
                        and t3.migrado = false
                        limit $count";

try {
    
    $visitantes = $db->carregar( $sqlVisitantes );
    $erroSql    = 0;
    
    if ($visitantes){
        
        $tipoPessoa = "Visitantes";
        $sqlPessoa  = $sqlVisitantes;
        
        for($i = 0; $i < count($visitantes); $i++){
            
            $visitante = $visitantes[$i];
            
            try{

                $foto = new FilesSimec("visitantefoto", array("vttid" => $visitante['vttid']) , "sca");
                $arquivoSalvo = $foto->setStream("Foto de visitante", pg_unescape_bytea($visitante['im_foto_visitante']));
                
                if($arquivoSalvo){
                    $db->executar( "update sca.".$visitante['tb_imagem']." set migrado=true where ".$visitante['img_id_cp']."='".$visitante['img_id']."'" );
                    $db->commit();
                }
                         
            }catch (Exception $erro){
                
                $erroSql++;
                
                $sqlErro = "INSERT INTO sca.logmigraimagem(logcampo, logcodigo, logtabela, logconsulta)
                            VALUES ('vttid', '".$visitante['vttid']."', '".$visitante['tb_imagem']."', '".addslashes( $sqlVisitantes )."');";
                $db->executar( $sqlErro );
                $db->commit();
            }
        }

    } else {
    
        $servidores = $db->carregar( $sqlServidores );
        $tipoPessoa = "Servidores";
        $sqlPessoa  = $sqlServidores;
    
        if ($servidores){

            for($i = 0; $i < count($servidores); $i++){
                $servidor = $servidores[$i];
    
                try{
                
                    $foto         = new FilesSimec("visitantefoto", array("nu_matricula_siape" => $servidor['nu_matricula_siape']) , "sca");
                    $arquivoSalvo = $foto->setStream("Foto de servidor", pg_unescape_bytea($servidor['foto']));
                    
                    if($arquivoSalvo){
                        exit($servidor['tb_imagem']);
                        $db->executar( "update sca.".$servidor['tb_imagem']." set migrado=true where nu_matricula_siape=".$servidor['nu_matricula_siape'] );
                        $db->commit();
                    }
                
                }catch (Exception $erro){
                    $erroSql++;
                    $sqlErro = "INSERT INTO sca.logmigraimagem(logcampo, logcodigo, logtabela, logconsulta)
                                VALUES ('nu_matricula_siape', '".$servidor['nu_matricula_siape']."', '".$servidor['tb_imagem']."', '".addslashes( $sqlServidores )."');";
                    $db->executar( $sqlErro );
                    $db->commit();
                }
            }
        }else{
            throw new Exception("Nada a importar!");
        }
    }
    
    if( $erroSql > 0 ){
        throw new Exception("Erro ao carregar ".$tipoPessoa.". Consulta: " . $sqlPessoa);
    }
    
}catch (Exception $e){
    
    $sqlErro = "INSERT INTO sca.logmigraimagem(logconsulta)
                VALUES ('".addslashes( $e->getMessage() )."');";
    
    $db->executar( $sqlErro );
    $db->commit();
}

die("Importa&ccedil;&atilde;o finalizada.");