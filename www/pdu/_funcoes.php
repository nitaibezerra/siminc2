<?PHP

    #icone que abre a árvore.
    function tree_acaoes_Plus(){
        echo "
            <span>
                <i class=\"glyphicon glyphicon-plus-sign\"></i>
            </span>
        ";
    }
    #icone que fecha a árvore.
    function tree_acaoes_Minus(){
        echo "
            <span>
                <i class=\"glyphicon glyphicon-minus-sign\"></i>
            </span>
        ";
    }

    function tree_acaoes_Add( $tipo, $id = NULL ){
        echo "
            <button type=\"button\" class=\"badge badge-success\" onclick=\"formulario_{$tipo}('{$id}' , this);\">
                <i class=\"glyphicon glyphicon-plus\"></i>
            </button>
        ";
    }

    function tree_acaoes_edit( $tipo, $id_pai = NULL, $id = NULL ){
        echo "
            <button type=\"button\" class=\"badge badge-warning\" onclick=\"edit_{$tipo}('{$id_pai}','{$id}' , this);\">
                <i class=\"glyphicon glyphicon-pencil\"></i>
            </button>
        ";

    }

    function tree_acaoes_del( $tipo, $id = NULL ){
        echo "
            <button type=\"button\" class=\"badge badge-important\" onclick=\"deletar_{$tipo}('{$id}');\">
                <i class=\"glyphicon glyphicon-trash\"></i>
            </button>
        ";
    }

    function tree_acaoes_serch( $tipo, $id = NULL ){
        echo "
            <button type=\"button\" class=\"badge badge-info\" onclick=\"abrir_dados_{$tipo}('{$id}');\">
                <i class=\"glyphicon glyphicon-search\"></i>
            </button>
        ";
    }

    function busca_isntrumento(){
        global $db;
        $sql = "
            SELECT itrid AS codigo, itrdsc AS descricao FROM pdu.instrumento
        ";
        return $db->carregar($sql);
    }

    function busca_dimensao( $coluna, $id ){
        global $db;

        if( $id != '' ){
            $where = "AND {$coluna} = {$id}";
        }
        $sql = "
            SELECT dimid AS codigo, dimdsc AS descricao FROM pdu.dimensao WHERE dimstatus = 'A' {$where} ORDER BY dimcod;
        ";
        return $db->carregar($sql);
    }

    function busca_area( $coluna, $id ){
        global $db;

        if( $id != '' ){
            $where = "AND {$coluna} = {$id}";
        }
        $sql = "
            SELECT  areid AS codigo, aredsc AS descricao FROM pdu.area WHERE arestatus = 'A' {$where} ORDER BY arecod;
        ";
        return $db->carregar($sql);
    }


    function busca_indicador( $coluna, $id ){
        global $db;

        if( $id != '' ){
            $where = "AND {$coluna} = {$id}";
        }
        $sql = "
            SELECT indid AS codigo, inddsc AS descricao FROM pdu.indicador WHERE indstatus = 'A' {$where} ORDER BY indcod;
        ";
        return $db->carregar($sql);
    }

    function busca_criterio( $coluna, $id ){
        global $db;

        if( $id != '' ){
            $where = "AND {$coluna} = {$id}";
        }
        $sql = "
            SELECT crtid AS codigo, crtdsc AS descricao FROM pdu.criterio WHERE crtstatus = 'A' {$where}
        ";
        return $db->carregar($sql);
    }
    
    function arvore_pdu(){
        global $db;

        $result_itrid = busca_isntrumento();

        echo "<div class=\"col-lg-12\">";
        
        echo "<div class=\"tree\" id=\"tree\">";

        echo "<ul>";
            echo "<li style=\"cursor:pointer;\">";
                echo "<span style=\"font-size:11px;\"> <i class=\"glyphicon glyphicon-minus-sign\" onclick=\"fecharTodos();\"> </i> </span>";
                echo " <label style=\"font-size:12px;\"> Fechar todos </label> ";
                
                echo " <span style=\"font-size:11px;\"> <i class=\"glyphicon glyphicon-plus-sign\" onclick=\"abrirTodos();\"> </i> </span>";
                echo " <label style=\"font-size:12px;\"> Abrir todos </label> ";
                
            echo "</li>";
            
            echo "<li>";
                echo "<i class=\"glyphicon glyphicon-sort-by-attributes\" style=\"font-size:16px; color:#428bca;\"> </i> ";
                echo " <spam class=\"\" style=\"font-size:15px;\"> GUIA DE AÇÕES PADRONIZADAS </spam>";
            echo "</li>";

        if( $result_itrid != '' ){

            foreach( $result_itrid as $instrumento ){

                echo "<li>";

                tree_acaoes_Plus();
                tree_acaoes_Add( 'dimensao', $instrumento['codigo'] );
                tree_acaoes_serch( 'dimensao', $instrumento['codigo'] );

                echo $instrumento['descricao'];

                $result_dimid = busca_dimensao( 'itrid', $instrumento['codigo'] );

                if( $result_dimid != '' ){
                    echo "<ul>";

                    foreach( $result_dimid as $dimesao ){
                        echo "<li>";

                        tree_acaoes_Minus();
                        tree_acaoes_Add( 'area', $dimesao['codigo'] );
                        tree_acaoes_edit( 'dimensao', $instrumento['codigo'], $dimesao['codigo'] );
                        tree_acaoes_del( 'dimensao', $dimesao['codigo'] );

                        echo $dimesao['descricao'];

                        $result_areid = busca_area( 'dimid', $dimesao['codigo'] );

                         if( $result_areid != '' ){
                            echo "<ul>";

                            foreach( $result_areid as $area ){
                                echo "<li>";

                                tree_acaoes_Minus();
                                tree_acaoes_Add( 'indicador', $area['codigo'] );
                                tree_acaoes_edit( 'area', $dimesao['codigo'], $area['codigo'] );
                                tree_acaoes_del( 'area', $area['codigo'] );

                                echo $area['descricao'];

                                $result_indid = busca_indicador( 'areid', $area['codigo'] );

                                if( $result_indid != '' ){
                                   echo "<ul>";

                                   foreach( $result_indid as $indicador ){
                                       echo "<li>";

                                       tree_acaoes_Minus();
                                       tree_acaoes_Add( 'criterio', $indicador['codigo'] );
                                       tree_acaoes_edit( 'indicador', $area['codigo'], $indicador['codigo'] );
                                       tree_acaoes_del( 'indicador', $indicador['codigo'] );

                                       echo $indicador['descricao'];

                                       $result_crtid = busca_criterio( 'indid', $indicador['codigo'] );

                                        if( $result_crtid != '' ){
                                           echo "<ul>";

                                           foreach( $result_crtid as $criterio ){
                                                echo "<li>";

                                                tree_acaoes_Minus();
                                                tree_acaoes_edit( 'criterio', $indicador['codigo'], $criterio['codigo'] );
                                                tree_acaoes_del( 'criterio', $criterio['codigo'] );

                                                echo $criterio['descricao'];

                                                echo "</li>";
                                           }

                                           echo "</ul>";

                                        }

                                       echo "</li>";

                                   }

                                   echo "</ul>";

                                }

                                echo "</li>";
                            }

                            echo "</ul>";
                         }

                        echo "</li>";
                    }

                    echo "</ul>";
                }//fim if - dimesão

                echo "</li>";

            }//fim - foreach isntrumento

        }//fim - if instrumento

        echo "</ul>";
        echo "</div>";
        echo "</div>";
    }
    
    
?>